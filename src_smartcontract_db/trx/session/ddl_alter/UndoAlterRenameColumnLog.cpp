/*
 * UndoAlterRenameColumn.cpp
 *
 *  Created on: 2023/08/04
 *      Author: iizuka
 */

#include "trx/session/ddl_alter/UndoAlterRenameColumnLog.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/TableObjectFactory.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "bc_base/BinaryUtils.h"

#include "engine/CdbBinaryObject.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "schema_table/schema/SchemaManager.h"
namespace codablecash {

UndoAlterRenameColumnLog::UndoAlterRenameColumnLog(const UndoAlterRenameColumnLog &inst)
		: AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_ALTER_RENAME_COLUMN) {
	this->table = inst.table != nullptr ? new CdbTable(*inst.table) : nullptr;
	this->lastName = inst.lastName != nullptr ? new UnicodeString(inst.lastName) : nullptr;
	this->newName = inst.newName != nullptr ? new UnicodeString(inst.newName) : nullptr;
}

UndoAlterRenameColumnLog::UndoAlterRenameColumnLog() : AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_ALTER_RENAME_COLUMN) {
	this->table = nullptr;
	this->lastName = nullptr;
	this->newName = nullptr;
}

UndoAlterRenameColumnLog::~UndoAlterRenameColumnLog() {
	delete this->table;
	delete this->lastName;
	delete this->newName;
}

int UndoAlterRenameColumnLog::binarySize() const {
	BinaryUtils::checkNotNull(this->table);
	BinaryUtils::checkNotNull(this->lastName);
	BinaryUtils::checkNotNull(this->newName);

	int total = sizeof(uint8_t);

	total += this->table->binarySize();
	total += BinaryUtils::stringSize(this->lastName);
	total += BinaryUtils::stringSize(this->newName);

	return total;
}

void UndoAlterRenameColumnLog::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->table);
	BinaryUtils::checkNotNull(this->lastName);
	BinaryUtils::checkNotNull(this->newName);

	out->put(this->type);

	this->table->toBinary(out);
	BinaryUtils::putString(out, this->lastName);
	BinaryUtils::putString(out, this->newName);
}

void UndoAlterRenameColumnLog::fromBinary(ByteBuffer *in) {
	{
		CdbBinaryObject* obj = TableObjectFactory::createFromBinary(in, CdbTable::CDB_OBJ_TYPE); __STP(obj);
		this->table = dynamic_cast<CdbTable*>(obj);

		this->table->fromBinary(in);

		BinaryUtils::checkNotNull(this->table);
		__STP_MV(obj);
	}

	this->lastName = BinaryUtils::getString(in);
	this->newName = BinaryUtils::getString(in);
}

IBlockObject* UndoAlterRenameColumnLog::copyData() const noexcept {
	return new UndoAlterRenameColumnLog(*this);
}

void UndoAlterRenameColumnLog::execute(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey *key) {
	SchemaManager* schemaManamger = trxManager->getSchemaManager();

	CdbTable* table = schemaManamger->getTable(this->table->getOid());
	assert(table != nullptr);

	table->renameColumn(this->newName, this->lastName);

	schemaManamger->save();
}

void UndoAlterRenameColumnLog::setTable(const CdbTable *tbl) noexcept {
	delete this->table;
	this->table = new CdbTable(*tbl);
}

void UndoAlterRenameColumnLog::setLastName(const UnicodeString *name) noexcept {
	delete this->lastName;
	this->lastName = new UnicodeString(name);
}

void UndoAlterRenameColumnLog::setNewName(const UnicodeString *name) noexcept {
	delete this->newName;
	this->newName = new UnicodeString(name);
}

} /* namespace codablecash */
