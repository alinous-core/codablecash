/*
 * UndoAlterAddIndexLog.cpp
 *
 *  Created on: 2023/07/24
 *      Author: iizuka
 */

#include "trx/session/ddl_alter/UndoAlterAddIndexLog.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "schema_table/schema/SchemaManager.h"

#include "schema_table/table/CdbTableIndex.h"
#include "schema_table/table/CdbTable.h"

#include "engine/CdbOid.h"

#include "base/StackRelease.h"

namespace codablecash {

UndoAlterAddIndexLog::UndoAlterAddIndexLog(const UndoAlterAddIndexLog &inst) : AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_ALTER_ADD_INDEX) {
	this->newIndexOid = inst.newIndexOid;
	this->tableOid = inst.tableOid;
}

UndoAlterAddIndexLog::UndoAlterAddIndexLog() : AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_ALTER_ADD_INDEX) {
	this->newIndexOid = 0L;
	this->tableOid = 0L;
}

UndoAlterAddIndexLog::~UndoAlterAddIndexLog() {

}

int UndoAlterAddIndexLog::binarySize() const {
	int total = sizeof(uint8_t);

	total += sizeof(this->newIndexOid);
	total += sizeof(this->tableOid);

	return total;
}

void UndoAlterAddIndexLog::toBinary(ByteBuffer *out) const {
	out->put(this->type);
	out->putLong(this->newIndexOid);
	out->putLong(this->tableOid);
}

void UndoAlterAddIndexLog::fromBinary(ByteBuffer *in) {
	this->newIndexOid = in->getLong();
	this->tableOid = in->getLong();
}

IBlockObject* UndoAlterAddIndexLog::copyData() const noexcept {
	return new UndoAlterAddIndexLog(*this);
}

void UndoAlterAddIndexLog::execute(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey *key) {
	SchemaManager* schemaManager = trxManager->getSchemaManager();

	CdbOid tableOid(this->tableOid);
	CdbOid indexOid(this->newIndexOid);

	CdbTable* table = schemaManager->getTable(&tableOid);

	CdbTableIndex* removalIndex = table->getIndex(&indexOid); __STP(removalIndex);
	assert(removalIndex != nullptr);

	table->removeIndex(removalIndex);

	schemaManager->fireOnDropIndex(trxManager, table, removalIndex);
	schemaManager->save();
}

void UndoAlterAddIndexLog::setNewIndexOid(uint64_t oid) noexcept {
	this->newIndexOid = oid;
}

void UndoAlterAddIndexLog::setTableOid(uint64_t oid) noexcept {
	this->tableOid = oid;
}

} /* namespace codablecash */
