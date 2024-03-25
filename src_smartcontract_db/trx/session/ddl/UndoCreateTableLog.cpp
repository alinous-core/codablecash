/*
 * UnodCreateTableLog.cpp
 *
 *  Created on: 2023/07/05
 *      Author: iizuka
 */
#include "trx/session/ddl/UndoCreateTableLog.h"

#include "engine/CdbOid.h"

#include "bc_base/BinaryUtils.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "schema_table/schema/SchemaManager.h"

#include "schema_table/table/CdbTable.h"

#include "base/UnicodeString.h"
namespace codablecash {

UndoCreateTableLog::UndoCreateTableLog(const UndoCreateTableLog &inst) : AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_CREATE_TABLE) {
	this->tableOid = inst.tableOid != nullptr ? inst.tableOid->copy() : nullptr;
}

UndoCreateTableLog::UndoCreateTableLog() : AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_CREATE_TABLE) {
	this->tableOid = nullptr;
}

UndoCreateTableLog::~UndoCreateTableLog() {
	delete this->tableOid;
}

int UndoCreateTableLog::binarySize() const {
	BinaryUtils::checkNotNull(this->tableOid);

	int total = sizeof(uint8_t);
	total += this->tableOid->binarySize();

	return total;
}

void UndoCreateTableLog::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->tableOid);

	out->put(this->type);
	this->tableOid->toBinary(out);
}

void UndoCreateTableLog::fromBinary(ByteBuffer *in) {
	this->tableOid = CdbOid::fromBinary(in);
	BinaryUtils::checkNotNull(this->tableOid);
}

IBlockObject* UndoCreateTableLog::copyData() const noexcept {
	return new UndoCreateTableLog(*this);
}

void UndoCreateTableLog::setTableOid(const CdbOid *oid) noexcept {
	delete this->tableOid;
	this->tableOid = oid->copy();
}

void UndoCreateTableLog::execute(CdbTransactionManager *trxManager, UndoLogPartsRepo* partsRepo, UndoLogsIdkey* key) {
	SchemaManager* scManager = trxManager->getSchemaManager();

	const CdbTable* table = scManager->getTable(this->tableOid);

	UnicodeString schemaName(table->getSchemaName());
	UnicodeString tableName(table->getName());

	scManager->removeTable(&schemaName, &tableName);
}

} /* namespace codablecash */
