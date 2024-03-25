/*
 * UndoAlterModifyLog.cpp
 *
 *  Created on: 2023/07/24
 *      Author: iizuka
 */

#include "trx/session/ddl_alter/UndoAlterModifyLog.h"

#include "schema_table/schema/SchemaManager.h"

#include "schema_table/table/CdbTableColumn.h"
#include "schema_table/table/CdbTable.h"

#include "schema_table/table_store/TableStore.h"
#include "schema_table/table_store/CdbStorageManager.h"

#include "trx/session/repo/UndoLogsIdkey.h"
#include "trx/session/repo/UndoLogPartsScanner.h"
#include "trx/session/repo/UndoLogPartsRepo.h"

#include "trx/session/base/CdbDatabaseSessionId.h"
#include "trx/session/base/AbstractUndoLogPart.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "engine/CdbOid.h"

#include "base/StackRelease.h"

#include "base/ArrayList.h"

#include "bc_base/BinaryUtils.h"

#include "schema_table/table/TableObjectFactory.h"
namespace codablecash {

UndoAlterModifyLog::UndoAlterModifyLog(const UndoAlterModifyLog &inst) : AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_ALTER_MODIFY) {
	this->tableOid = inst.tableOid;
	this->column = inst.column != nullptr ? new CdbTableColumn(*inst.column) : nullptr;
	this->dataModified = inst.dataModified;
}

UndoAlterModifyLog::UndoAlterModifyLog() : AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_ALTER_MODIFY) {
	this->tableOid = 0L;
	this->column = nullptr;
	this->dataModified = false;
}

UndoAlterModifyLog::~UndoAlterModifyLog() {
	delete this->column;
}

int UndoAlterModifyLog::binarySize() const {
	BinaryUtils::checkNotNull(this->column);

	int total = sizeof(uint8_t);

	total += sizeof(this->tableOid);
	total += this->column->binarySize();
	total += sizeof(uint8_t);

	return total;
}

void UndoAlterModifyLog::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->column);

	out->put(this->type);
	out->putLong(this->tableOid);

	this->column->toBinary(out);

	out->put(this->dataModified ? 1 : 0);
}

void UndoAlterModifyLog::fromBinary(ByteBuffer *in) {
	this->tableOid = in->getLong();

	CdbBinaryObject* obj = TableObjectFactory::createFromBinary(in, CdbTableColumn::CDB_OBJ_TYPE); __STP(obj);
	this->column = dynamic_cast<CdbTableColumn*>(obj);
	BinaryUtils::checkNotNull(this->column);
	__STP_MV(obj);

	this->column->fromBinary(in);

	uint8_t bl = in->get();
	this->dataModified = bl > 0;
}

IBlockObject* UndoAlterModifyLog::copyData() const noexcept {
	return new UndoAlterModifyLog(*this);
}

void UndoAlterModifyLog::execute(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey *key) {
	SchemaManager* schemaManager = trxManager->getSchemaManager();

	CdbOid oid(this->tableOid);
	CdbTable* table = schemaManager->getTable(&oid);
	assert(table != nullptr);

	CdbTableColumn* col = table->getColumn(this->column->getOid());

	col->setAttributes(this->column->isNotnull(), this->column->isUnique());
	col->setDefaultValue(this->column->getDefaultValue());
	col->setType(this->column->getType(), this->column->getLength());

	schemaManager->save();

	// unique indexes and records
	handleParts(trxManager, partsRepo, key);
}

void UndoAlterModifyLog::setTableOid(uint64_t oid) noexcept {
	this->tableOid = oid;
}

void UndoAlterModifyLog::setColumn(const CdbTableColumn *col) noexcept {
	delete this->column;
	this->column = new CdbTableColumn(*col);
}

void UndoAlterModifyLog::setDataModified(bool bl) noexcept {
	this->dataModified = bl;
}

void UndoAlterModifyLog::handleParts(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey *key) {
	CdbStorageManager* storageManager = trxManager->getStorageManager();

	CdbOid tableOid(this->tableOid);
	TableStore* store = storageManager->getTableStore(&tableOid);

	if(this->dataModified){
		store->resetAllIndexes();
	}

	const CdbDatabaseSessionId* sessionId = key->getSessionId();
	uint64_t undoLogSerial = key->getUndoLogSerial()->getOidValue();

	ArrayList<AbstractUndoLogPart> afterList;
	afterList.setDeleteOnExit();

	UndoLogPartsScanner* scanner = partsRepo->getScanner(sessionId, undoLogSerial); __STP(scanner);
	while(scanner->hasNext()){
		const AbstractUndoLogPart* part = scanner->next();

		if(AbstractUndoLogPart::PART_UNDO_REMOVE_INDEX == part->getType()){
			AbstractUndoLogPart* p = dynamic_cast<AbstractUndoLogPart*>(part->copyData());
			afterList.addElement(p);
			continue;
		}

		part->execute(trxManager, store);
	}

	if(this->dataModified){
		store->buildAllIndexes();
	}

	{
		int maxLoop = afterList.size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractUndoLogPart* part = afterList.get(i);

			part->execute(trxManager, store);
		}
	}
}

} /* namespace codablecash */
