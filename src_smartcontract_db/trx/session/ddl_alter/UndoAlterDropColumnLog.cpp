/*
 * UndoAlterDropCplumnLog.cpp
 *
 *  Created on: 2023/07/31
 *      Author: iizuka
 */

#include "trx/session/ddl_alter/UndoAlterDropColumnLog.h"

#include "trx/session/repo/UndoLogsIdkey.h"
#include "trx/session/repo/UndoLogPartsScanner.h"
#include "trx/session/repo/UndoLogPartsRepo.h"

#include "trx/session/base/AbstractUndoLogPart.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/CdbTableColumn.h"
#include "schema_table/table/TableObjectFactory.h"

#include "schema_table/schema/SchemaManager.h"

#include "schema_table/table_store/CdbStorageManager.h"
#include "schema_table/table_store/TableStore.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

#include "engine/CdbOid.h"

#include "base/ArrayList.h"

namespace codablecash {

UndoAlterDropColumnLog::UndoAlterDropColumnLog(const UndoAlterDropColumnLog &inst) : AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_ALTER_DROP_COLUMN) {
	this->table = inst.table != nullptr ? new CdbTable(*inst.table) : nullptr;
	this->removedColumn = inst.removedColumn != nullptr ? new CdbTableColumn(*inst.removedColumn) : nullptr;
}

UndoAlterDropColumnLog::UndoAlterDropColumnLog() : AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_ALTER_DROP_COLUMN) {
	this->table = nullptr;
	this->removedColumn = nullptr;
}

UndoAlterDropColumnLog::~UndoAlterDropColumnLog() {
	delete this->table;
	delete this->removedColumn;
}

int UndoAlterDropColumnLog::binarySize() const {
	BinaryUtils::checkNotNull(this->table);
	BinaryUtils::checkNotNull(this->removedColumn);

	int total = sizeof(uint8_t);

	total += this->table->binarySize();
	total += this->removedColumn->binarySize();

	return total;
}

void UndoAlterDropColumnLog::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->table);
	BinaryUtils::checkNotNull(this->removedColumn);

	out->put(this->type);

	this->table->toBinary(out);
	this->removedColumn->toBinary(out);
}

void UndoAlterDropColumnLog::fromBinary(ByteBuffer *in) {
	{
		CdbBinaryObject* obj = TableObjectFactory::createFromBinary(in, CdbTable::CDB_OBJ_TYPE); __STP(obj);
		this->table = dynamic_cast<CdbTable*>(obj);

		this->table->fromBinary(in);

		BinaryUtils::checkNotNull(this->table);
		__STP_MV(obj);
	}

	{
		CdbBinaryObject* obj = TableObjectFactory::createFromBinary(in, CdbTableColumn::CDB_OBJ_TYPE); __STP(obj);
		this->removedColumn = dynamic_cast<CdbTableColumn*>(obj);
		BinaryUtils::checkNotNull(this->removedColumn);
		__STP_MV(obj);

		this->removedColumn->fromBinary(in);
	}
}

IBlockObject* UndoAlterDropColumnLog::copyData() const noexcept {
	return new UndoAlterDropColumnLog(*this);
}

void UndoAlterDropColumnLog::execute(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey *key) {
	SchemaManager* manager = trxManager->getSchemaManager();

	CdbOid tableOid(*this->table->getOid());
	CdbTable* table = manager->getTable(&tableOid);

	int pos = this->removedColumn->getPosition();
	CdbTableColumn* col = new CdbTableColumn(*this->removedColumn);
	table->insertColumn(col, pos);

	manager->save();

	handleParts(trxManager, partsRepo, key);
}

void UndoAlterDropColumnLog::handleParts(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey *key) {
	CdbStorageManager* storageManager = trxManager->getStorageManager();

	TableStore* store = storageManager->getTableStore(this->table->getOid());

	store->resetAllIndexes(); // delete indexes

	const CdbDatabaseSessionId* sessionId = key->getSessionId();
	uint64_t undoLogSerial = key->getUndoLogSerial()->getOidValue();

	ArrayList<AbstractUndoLogPart> list;
	list.setDeleteOnExit();

	UndoLogPartsScanner* scanner = partsRepo->getScanner(sessionId, undoLogSerial); __STP(scanner);
	while(scanner->hasNext()){
		const AbstractUndoLogPart* part = scanner->next();

		if(part->getType() == AbstractUndoLogPart::PART_UNDO_REMOVE_INDEX){
			AbstractUndoLogPart* p = dynamic_cast<AbstractUndoLogPart*>(part->copyData());
			list.addElement(p);
			continue;
		}

		part->execute(trxManager, store);
	}

	store->buildAllIndexes(); // build indexes

	int maxLoop = list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractUndoLogPart* part = list.get(i);
		part->execute(trxManager, store);
	}
}

void UndoAlterDropColumnLog::setTable(const CdbTable *table) noexcept {
	delete this->table;
	this->table = new CdbTable(*table);
}

void UndoAlterDropColumnLog::setRemoveColumn(const CdbTableColumn *col) noexcept {
	delete this->removedColumn;
	this->removedColumn = new CdbTableColumn(*col);
}

} /* namespace codablecash */
