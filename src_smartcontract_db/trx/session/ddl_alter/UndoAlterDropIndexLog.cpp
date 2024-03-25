/*
 * UndoAlterDropIndexLog.cpp
 *
 *  Created on: 2023/07/24
 *      Author: iizuka
 */

#include "trx/session/ddl_alter/UndoAlterDropIndexLog.h"

#include "schema_table/table/CdbTableIndex.h"

#include "bc_base/BinaryUtils.h"

#include "schema_table/table/TableObjectFactory.h"

#include "schema_table/table/CdbTable.h"

#include "base/StackRelease.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "schema_table/schema/SchemaManager.h"
namespace codablecash {

UndoAlterDropIndexLog::UndoAlterDropIndexLog(const UndoAlterDropIndexLog &inst) : AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_ALTER_DROP_INDEX) {
	this->removalIndex = inst.removalIndex != nullptr ? new CdbTableIndex(*inst.removalIndex) : nullptr;
	this->table = inst.table != nullptr ? new CdbTable(*inst.table) : nullptr;
}

UndoAlterDropIndexLog::UndoAlterDropIndexLog() : AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_ALTER_DROP_INDEX) {
	this->removalIndex = nullptr;
	this->table = nullptr;
}

UndoAlterDropIndexLog::~UndoAlterDropIndexLog() {
	delete this->removalIndex;
	delete this->table;
}

int UndoAlterDropIndexLog::binarySize() const {
	BinaryUtils::checkNotNull(this->removalIndex);
	BinaryUtils::checkNotNull(this->table);

	int total = sizeof(uint8_t);

	total += this->table->binarySize();
	total += this->removalIndex->binarySize();

	return total;
}

void UndoAlterDropIndexLog::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->removalIndex);
	BinaryUtils::checkNotNull(this->table);

	out->put(this->type);
	this->table->toBinary(out);
	this->removalIndex->toBinary(out);
}

void UndoAlterDropIndexLog::fromBinary(ByteBuffer *in) {
	{
		CdbBinaryObject* obj = TableObjectFactory::createFromBinary(in, CdbTable::CDB_OBJ_TYPE); __STP(obj);
		this->table = dynamic_cast<CdbTable*>(obj);

		this->table->fromBinary(in);

		BinaryUtils::checkNotNull(this->table);
		__STP_MV(obj);
	}

	{
		CdbBinaryObject* obj = TableObjectFactory::createFromBinary(in, CdbTableIndex::CDB_OBJ_TYPE); __STP(obj);
		this->removalIndex = dynamic_cast<CdbTableIndex*>(obj);
		this->removalIndex->fromBinary(in, this->table);

		BinaryUtils::checkNotNull(this->removalIndex);
		__STP_MV(obj);
	}
}

IBlockObject* UndoAlterDropIndexLog::copyData() const noexcept {
	return new UndoAlterDropIndexLog(*this);
}

void UndoAlterDropIndexLog::execute(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey *key) {
	SchemaManager* schemaManager = trxManager->getSchemaManager();

	const UnicodeString* schemaName = this->table->getSchemaName();
	const UnicodeString* tableName = this->table->getName();

	CdbTableIndex *index = new CdbTableIndex(*this->removalIndex);

	schemaManager->recoverIndex(trxManager, index, schemaName, tableName);
}

void UndoAlterDropIndexLog::setTable(const CdbTable* table) noexcept {
	delete this->table;
	this->table = new CdbTable(*table);
}

void UndoAlterDropIndexLog::setTableIndex(const CdbTableIndex *removalIndex) noexcept {
	delete this->removalIndex;
	this->removalIndex = new CdbTableIndex(*removalIndex);
}

} /* namespace codablecash */
