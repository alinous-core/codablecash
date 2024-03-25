/*
 * AlterRenameTableCommandLog.cpp
 *
 *  Created on: 2020/09/10
 *      Author: iizuka
 */

#include "trx/transaction_log_alter_modify/AlterRenameTableCommandLog.h"

#include "lang_sql/sql_ddl_alter_modify/AlterRenameTableCommand.h"

#include "engine_lock/WriteLockHandle.h"
#include "engine_lock/StackDbLockUnlocker.h"
#include "engine_lock/ReadLockHandle.h"

#include "base/StackRelease.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "lang_sql/sql_join_parts/TableIdentifier.h"

#include "vm/VirtualMachine.h"

#include "engine/CodableDatabase.h"


namespace codablecash {

AlterRenameTableCommandLog::AlterRenameTableCommandLog(uint64_t oid) : AbstractAlterCommandLog(AbstractTransactionLog::TRX_ALTER_RENAME_TABLE, oid) {
	this->command = nullptr;
}

AlterRenameTableCommandLog::~AlterRenameTableCommandLog() {
	delete this->command;
}

void AlterRenameTableCommandLog::setCommand(AlterRenameTableCommand* command) noexcept {
	this->command = command;
}

int AlterRenameTableCommandLog::binarySize() const {
	CodeElement::checkNotNull(this->command);

	int total = sizeof(uint8_t) + sizeof(uint64_t);

	total += AbstractAlterCommandLog::binarySize();
	total += this->command->binarySize();

	return total;;
}

void AlterRenameTableCommandLog::toBinary(ByteBuffer* out) const {
	CodeElement::checkNotNull(this->command);

	out->put(AbstractTransactionLog::TRX_ALTER_RENAME_TABLE);
	out->putLong(this->oid);

	AbstractAlterCommandLog::toBinary(out);
	this->command->toBinary(out);
}

void AlterRenameTableCommandLog::fromBinary(ByteBuffer* in) {
	AbstractAlterCommandLog::fromBinary(in);

	CodeElement* element = CodeElement::createFromBinary(in);
	CodeElement::checkKind(element, CodeElement::DDL_ALTER_RENAME_TABLE);

	this->command = dynamic_cast<AlterRenameTableCommand*>(element);
}

void AlterRenameTableCommandLog::commit(CdbTransactionManager* trxManager, CdbTransaction* trx) {
	WriteLockHandle* lockH = trxManager->databaseWriteLock();
	StackDbLockUnlocker unlocker(lockH);

	trxManager->commitAlterTable(this);
}

void AlterRenameTableCommandLog::initCommandParam(VirtualMachine* vm, TableIdentifier* tableId) {
	CodableDatabase* db = vm->getDb();
	ReadLockHandle* lockH = db->databaseReadLock();
	StackDbLockUnlocker unclocker(lockH);

	this->command->interpret(vm, this, tableId);
}

void AlterRenameTableCommandLog::inputDefaultSchema(const UnicodeString* defaultSchema) {
	this->tableId->inputDefaultSchema(defaultSchema);
	this->command->inputDefaultSchema(defaultSchema);

}


} /* namespace codablecash */
