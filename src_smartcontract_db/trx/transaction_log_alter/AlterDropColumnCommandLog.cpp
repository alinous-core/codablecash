/*
 * AlterDropColumnCommandLog.cpp
 *
 *  Created on: 2020/09/10
 *      Author: iizuka
 */

#include "trx/transaction_log_alter/AlterDropColumnCommandLog.h"

#include "lang_sql/sql_ddl_alter/AlterDropColumnCommand.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "engine_lock/WriteLockHandle.h"
#include "engine_lock/StackDbLockUnlocker.h"
#include "engine_lock/ReadLockHandle.h"

#include "base/StackRelease.h"

#include "vm/VirtualMachine.h"

#include "engine/CodableDatabase.h"


namespace codablecash {

AlterDropColumnCommandLog::AlterDropColumnCommandLog(uint64_t oid) : AbstractAlterCommandLog(AbstractTransactionLog::TRX_ALTER_DROP_COLUMN, oid) {
	this->command = nullptr;
}

AlterDropColumnCommandLog::~AlterDropColumnCommandLog() {
	delete this->command;
}

void AlterDropColumnCommandLog::setCommand(AlterDropColumnCommand* command) noexcept {
	this->command = command;
}

int AlterDropColumnCommandLog::binarySize() const {
	CodeElement::checkNotNull(this->command);

	int total = sizeof(uint8_t) + sizeof(uint64_t);

	total += AbstractAlterCommandLog::binarySize();
	total += this->command->binarySize();

	return total;
}

void AlterDropColumnCommandLog::toBinary(ByteBuffer* out) const {
	CodeElement::checkNotNull(this->command);

	out->put(AbstractTransactionLog::TRX_ALTER_DROP_COLUMN);
	out->putLong(this->oid);

	AbstractAlterCommandLog::toBinary(out);
	this->command->toBinary(out);
}

void AlterDropColumnCommandLog::fromBinary(ByteBuffer* in) {
	AbstractAlterCommandLog::fromBinary(in);

	CodeElement* element = CodeElement::createFromBinary(in);
	CodeElement::checkKind(element, CodeElement::DDL_ALTER_DROP_COLUMN);

	this->command = dynamic_cast<AlterDropColumnCommand*>(element);
}

void AlterDropColumnCommandLog::commit(CdbTransactionManager* trxManager, CdbTransaction* trx) {
	WriteLockHandle* lockH = trxManager->databaseWriteLock();
	StackDbLockUnlocker unlocker(lockH);

	trxManager->commitAlterTable(this);
}

void AlterDropColumnCommandLog::initCommandParam(VirtualMachine* vm, TableIdentifier* tableId) {
	CodableDatabase* db = vm->getDb();
	ReadLockHandle* lockH = db->databaseReadLock();
	StackDbLockUnlocker unclocker(lockH);

	this->command->interpret(vm, this, tableId);
}

} /* namespace codablecash */
