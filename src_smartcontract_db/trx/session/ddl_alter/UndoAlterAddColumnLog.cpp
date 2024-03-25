/*
 * UndoAlterAddColumnLog.cpp
 *
 *  Created on: 2023/07/31
 *      Author: iizuka
 */

#include "trx/session/ddl_alter/UndoAlterAddColumnLog.h"

#include "trx/session/repo/UndoLogsIdkey.h"
#include "trx/session/repo/UndoLogPartsScanner.h"
#include "trx/session/repo/UndoLogPartsRepo.h"

#include "trx/session/base/AbstractUndoLogPart.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/TableObjectFactory.h"
#include "schema_table/table/CdbTableColumn.h"

#include "schema_table/schema/SchemaManager.h"

#include "schema_table/table_store/CdbStorageManager.h"
#include "schema_table/table_store/AlterRecordValueExecutor.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "engine/CdbOid.h"


namespace codablecash {

UndoAlterAddColumnLog::UndoAlterAddColumnLog(const UndoAlterAddColumnLog &inst) : AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_ALTER_ADD_COLUMN) {
	this->table = inst.table != nullptr ? new CdbTable(*inst.table) : nullptr;
	this->newOid = inst.newOid;
}

UndoAlterAddColumnLog::UndoAlterAddColumnLog() : AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_ALTER_ADD_COLUMN) {
	this->table = nullptr;
	this->newOid = 0L;
}

UndoAlterAddColumnLog::~UndoAlterAddColumnLog() {
	delete this->table;
}

int UndoAlterAddColumnLog::binarySize() const {
	BinaryUtils::checkNotNull(this->table);

	int total = sizeof(uint8_t);
	total += this->table->binarySize();
	total += sizeof(this->newOid);

	return total;
}

void UndoAlterAddColumnLog::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->table);

	out->put(this->type);
	this->table->toBinary(out);
	out->putLong(this->newOid);
}

void UndoAlterAddColumnLog::fromBinary(ByteBuffer *in) {
	{
		CdbBinaryObject* obj = TableObjectFactory::createFromBinary(in, CdbTable::CDB_OBJ_TYPE); __STP(obj);
		this->table = dynamic_cast<CdbTable*>(obj);

		this->table->fromBinary(in);

		BinaryUtils::checkNotNull(this->table);
		__STP_MV(obj);
	}

	this->newOid = in->getLong();
}

IBlockObject* UndoAlterAddColumnLog::copyData() const noexcept {
	return new UndoAlterAddColumnLog(*this);
}

void UndoAlterAddColumnLog::execute(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey *key) {
	SchemaManager* schemaManager = trxManager->getSchemaManager();
	CdbStorageManager* storageManager = trxManager->getStorageManager();

	{
		CdbTable* currentTable = schemaManager->getTable(this->table->getOid());

		CdbOid columnOid(this->newOid);
		CdbTableColumn* col = currentTable->getColumn(&columnOid);
		assert(col != nullptr);

		CdbTableColumn* removedCol = currentTable->removeColumn(col);
		assert(col == removedCol);
		delete col;

		schemaManager->save();
	}

	TableStore* store = storageManager->getTableStore(this->table->getOid());
	{
		int removepos = getRemoveColumnPos();

		AlterRecordValueExecutor exec(removepos);
		exec.removeColumn(store);
	}

	handleParts(trxManager, partsRepo, key);
}

void UndoAlterAddColumnLog::handleParts(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey *key) {
	CdbStorageManager* storageManager = trxManager->getStorageManager();

	TableStore* store = storageManager->getTableStore(this->table->getOid());

	const CdbDatabaseSessionId* sessionId = key->getSessionId();
	uint64_t undoLogSerial = key->getUndoLogSerial()->getOidValue();

	UndoLogPartsScanner* scanner = partsRepo->getScanner(sessionId, undoLogSerial); __STP(scanner);
	while(scanner->hasNext()){
		const AbstractUndoLogPart* part = scanner->next();

		part->execute(trxManager, store);
	}
}

int UndoAlterAddColumnLog::getRemoveColumnPos() const noexcept {
	const ArrayList<CdbTableColumn>* list = this->table->getColumns();
	return list->size();
}

void UndoAlterAddColumnLog::setTable(const CdbTable *table) noexcept {
	delete this->table;
	this->table = new CdbTable(*table);
}

void UndoAlterAddColumnLog::setnewOid(uint64_t oid) {
	this->newOid = oid;
}

} /* namespace codablecash */
