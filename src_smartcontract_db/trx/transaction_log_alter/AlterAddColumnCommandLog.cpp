/*
 * AlterAddColumnCommandLog.cpp
 *
 *  Created on: 2020/09/10
 *      Author: iizuka
 */

#include "trx/transaction_log_alter/AlterAddColumnCommandLog.h"

#include "lang_sql/sql_ddl_alter/AlterAddColumnCommand.h"

#include "engine/sc/CodeElement.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "engine_lock/WriteLockHandle.h"
#include "engine_lock/StackDbLockUnlocker.h"

#include "base/StackRelease.h"

#include "engine/CodableDatabase.h"

#include "engine_lock/ReadLockHandle.h"

#include "vm/VirtualMachine.h"
namespace codablecash {

AlterAddColumnCommandLog::AlterAddColumnCommandLog(uint64_t oid) : AbstractAlterCommandLog(AbstractTransactionLog::TRX_ALTER_ADD_COLUMN, oid) {
	this->command = nullptr;
}

AlterAddColumnCommandLog::~AlterAddColumnCommandLog() {
	delete this->command;
}

void AlterAddColumnCommandLog::setCommand(AlterAddColumnCommand* command) noexcept {
	this->command = command;
}

int AlterAddColumnCommandLog::binarySize() const {
	CodeElement::checkNotNull(this->command);

	int total = sizeof(uint8_t) + sizeof(uint64_t);

	total += AbstractAlterCommandLog::binarySize();
	total += this->command->binarySize();

	return total;
}

void AlterAddColumnCommandLog::toBinary(ByteBuffer* out) const {
	CodeElement::checkNotNull(this->command);

	out->put(AbstractTransactionLog::TRX_ALTER_ADD_COLUMN);
	out->putLong(this->oid);

	AbstractAlterCommandLog::toBinary(out);
	this->command->toBinary(out);
}

void AlterAddColumnCommandLog::fromBinary(ByteBuffer* in) {
	AbstractAlterCommandLog::fromBinary(in);

	CodeElement* element = CodeElement::createFromBinary(in);
	CodeElement::checkKind(element, CodeElement::DDL_ALTER_ADD_COLUMN);

	this->command = dynamic_cast<AlterAddColumnCommand*>(element);
}

void AlterAddColumnCommandLog::commit(CdbTransactionManager* trxManager, CdbTransaction* trx) {
	WriteLockHandle* lockH = trxManager->databaseWriteLock();
	StackDbLockUnlocker unlocker(lockH);

	trxManager->commitAlterTable(this);
}

void AlterAddColumnCommandLog::reanalyze(AnalyzeContext* ctx, CodeElement* parent) {
	this->command->setParent(parent);
	this->command->preAnalyze(ctx);

	this->command->analyzeTypeRef(ctx);

	this->command->analyze(ctx);
}

void AlterAddColumnCommandLog::initCommandParam(VirtualMachine* vm, TableIdentifier* tableId) {
	CodableDatabase* db = vm->getDb();
	ReadLockHandle* lockH = db->databaseReadLock();
	StackDbLockUnlocker unclocker(lockH);

	this->command->interpret(vm, this, tableId);
}

} /* namespace codablecash */
