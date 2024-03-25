/*
 * AlterRenameColumnCommandLog.cpp
 *
 *  Created on: 2020/09/10
 *      Author: iizuka
 */

#include "trx/transaction_log_alter_modify/AlterRenameColumnCommandLog.h"

#include "lang_sql/sql_ddl_alter_modify/AlterRenameColumnCommand.h"

#include "engine_lock/WriteLockHandle.h"
#include "engine_lock/StackDbLockUnlocker.h"
#include "engine_lock/ReadLockHandle.h"

#include "base/StackRelease.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "vm/VirtualMachine.h"

#include "engine/CodableDatabase.h"


namespace codablecash {

AlterRenameColumnCommandLog::AlterRenameColumnCommandLog(uint64_t oid) : AbstractAlterCommandLog(AbstractTransactionLog::TRX_ALTER_RENAME_COLUMN, oid) {
	this->command = nullptr;
}

AlterRenameColumnCommandLog::~AlterRenameColumnCommandLog() {
	delete this->command;
}

void AlterRenameColumnCommandLog::setCommand(AlterRenameColumnCommand* command) noexcept {
	this->command = command;
}

int AlterRenameColumnCommandLog::binarySize() const {
	CodeElement::checkNotNull(this->command);

	int total = sizeof(uint8_t) + sizeof(uint64_t);

	total += AbstractAlterCommandLog::binarySize();
	total += this->command->binarySize();

	return total;
}

void AlterRenameColumnCommandLog::toBinary(ByteBuffer* out) const {
	CodeElement::checkNotNull(this->command);

	out->put(AbstractTransactionLog::TRX_ALTER_RENAME_COLUMN);
	out->putLong(this->oid);

	AbstractAlterCommandLog::toBinary(out);
	this->command->toBinary(out);
}

void AlterRenameColumnCommandLog::fromBinary(ByteBuffer* in) {
	AbstractAlterCommandLog::fromBinary(in);

	CodeElement* element = CodeElement::createFromBinary(in);
	CodeElement::checkKind(element, CodeElement::DDL_ALTER_RENAME_COLUMN);

	this->command = dynamic_cast<AlterRenameColumnCommand*>(element);
}

void AlterRenameColumnCommandLog::commit(CdbTransactionManager* trxManager, CdbTransaction* trx) {
	WriteLockHandle* lockH = trxManager->databaseWriteLock();
	StackDbLockUnlocker unlocker(lockH);

	trxManager->commitAlterTable(this);
}

void AlterRenameColumnCommandLog::initCommandParam(VirtualMachine* vm, TableIdentifier* tableId) {
	CodableDatabase* db = vm->getDb();
	ReadLockHandle* lockH = db->databaseReadLock();
	StackDbLockUnlocker unclocker(lockH);

	this->command->interpret(vm, this, tableId);
}

} /* namespace codablecash */
