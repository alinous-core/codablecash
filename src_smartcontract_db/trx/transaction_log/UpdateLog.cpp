/*
 * UpdateLog.cpp
 *
 *  Created on: 2022/01/09
 *      Author: iizuka
 */

#include "trx/transaction_log/UpdateLog.h"

#include "schema_table/record/table_record/CdbRecord.h"
#include "schema_table/record/table_record/CdbTableIdentifier.h"

#include "base_io/ByteBuffer.h"

#include "engine_lock/WriteLockHandle.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "engine_lock/StackDbLockUnlocker.h"

#include "trx/session/base/CdbDatabaseSession.h"
#include "trx/session/base/UndoSpaceManager.h"
#include "trx/session/base/UndoRegisterTransaction.h"

#include "trx/session/dml/UndoUpdateLog.h"

namespace codablecash {

UpdateLog::UpdateLog(uint64_t oid) : AbstractTransactionLog(AbstractTransactionLog::TRX_UPDATE, oid) {
	this->table = nullptr;
}

UpdateLog::~UpdateLog() {
	delete this->table;
	this->records.deleteElements();
}

int UpdateLog::binarySize() const {
	checkNotNull(this->table);

	int total = sizeof(uint8_t) + sizeof(uint64_t);

	total += this->table->binarySize();

	int maxLoop = this->records.size();
	total += sizeof(int32_t);

	for(int i = 0; i != maxLoop; ++i){
		CdbRecord* rec = this->records.get(i);

		total += rec->binarySize();
	}

	return total;
}

void UpdateLog::toBinary(ByteBuffer* out) const {
	checkNotNull(this->table);

	out->put(AbstractTransactionLog::TRX_UPDATE);
	out->putLong(this->oid);

	this->table->toBinary(out);

	int maxLoop = this->records.size();
	out->putInt(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		CdbRecord* rec = this->records.get(i);

		rec->toBinary(out);
	}
}

void UpdateLog::fromBinary(ByteBuffer* in) {
	this->table = CdbTableIdentifier::fromBinary(in);

	int maxLoop = in->getInt();
	for(int i = 0; i != maxLoop; ++i){
		CdbRecord* rec = CdbRecord::createFromBinary(in);
		addRecord(rec);
	}
}

void UpdateLog::setTable(CdbTableIdentifier* table) noexcept {
	this->table = table;
}

void UpdateLog::addRecord(CdbRecord* record) noexcept {
	record->clearScanIdentity();
	this->records.addElement(record);
}

void UpdateLog::commit(CdbTransactionManager* trxManager, CdbTransaction* trx) {
	CdbDatabaseSession* session = trxManager->getCdbDatabaseSession();
	UndoSpaceManager* undoManager = session->getUndoSpaceManager();

	{
		WriteLockHandle* lockH = trxManager->databaseWriteLock();
		StackDbLockUnlocker unlocker(lockH);

		//　Undo Update Log
		UndoRegisterTransaction undoTrx(undoManager);

		UndoUpdateLog log;
		log.setTableId(this->table);
		session->addUndoLog(&log);

		trxManager->commitUpdate(this, trx, session);

		undoTrx.commit();
	}

}

} /* namespace codablecash */
