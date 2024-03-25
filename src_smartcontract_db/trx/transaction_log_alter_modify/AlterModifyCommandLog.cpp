/*
 * AlterModifyCommandLog.cpp
 *
 *  Created on: 2020/09/10
 *      Author: iizuka
 */

#include "trx/transaction_log_alter_modify/AlterModifyCommandLog.h"

#include "lang_sql/sql_ddl_alter_modify/AlterModifyCommand.h"

#include "engine_lock/WriteLockHandle.h"
#include "engine_lock/StackDbLockUnlocker.h"
#include "engine_lock/ReadLockHandle.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "vm/VirtualMachine.h"

#include "engine/CodableDatabase.h"


namespace codablecash {

AlterModifyCommandLog::AlterModifyCommandLog(uint64_t oid) : AbstractAlterCommandLog(AbstractTransactionLog::TRX_ALTER_MODIFY, oid) {
	this->command = nullptr;
	this->length = 0;
}

AlterModifyCommandLog::~AlterModifyCommandLog() {
	delete this->command;
}

void AlterModifyCommandLog::setCommand(AlterModifyCommand* command) noexcept {
	this->command = command;
}

int AlterModifyCommandLog::binarySize() const {
	CodeElement::checkNotNull(this->command);

	int total = sizeof(uint8_t) + sizeof(uint64_t);

	total += AbstractAlterCommandLog::binarySize();
	total += this->command->binarySize();

	total += sizeof(int64_t); // length

	return total;
}

void AlterModifyCommandLog::toBinary(ByteBuffer* out) const {
	CodeElement::checkNotNull(this->command);

	out->put(AbstractTransactionLog::TRX_ALTER_MODIFY);
	out->putLong(this->oid);

	AbstractAlterCommandLog::toBinary(out);
	this->command->toBinary(out);

	out->putLong(this->length);
}

void AlterModifyCommandLog::fromBinary(ByteBuffer* in) {
	AbstractAlterCommandLog::fromBinary(in);

	CodeElement* element = CodeElement::createFromBinary(in);
	CodeElement::checkKind(element, CodeElement::DDL_ALTER_MODIFY);

	this->command = dynamic_cast<AlterModifyCommand*>(element);

	this->length = in->getLong();
}

void AlterModifyCommandLog::commit(CdbTransactionManager* trxManager, CdbTransaction* trx) {
	WriteLockHandle* lockH = trxManager->databaseWriteLock();
	StackDbLockUnlocker unlocker(lockH);

	trxManager->commitAlterTable(this);
}

void AlterModifyCommandLog::initCommandParam(VirtualMachine* vm, TableIdentifier* tableId) {
	CodableDatabase* db = vm->getDb();
	ReadLockHandle* lockH = db->databaseReadLock();
	StackDbLockUnlocker unclocker(lockH);

	this->command->interpret(vm, this, tableId);
}

void AlterModifyCommandLog::setLength(int64_t length) noexcept {
	this->length = length;
}

void AlterModifyCommandLog::reanalyze(AnalyzeContext* ctx, CodeElement* parent) {
	this->command->setParent(parent);
	this->command->preAnalyze(ctx);

	this->command->analyzeTypeRef(ctx);

	this->command->analyze(ctx);
}


} /* namespace codablecash */
