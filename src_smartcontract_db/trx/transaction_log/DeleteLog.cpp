/*
 * DeleteLog.cpp
 *
 *  Created on: 2022/01/09
 *      Author: iizuka
 */

#include "trx/transaction_log/DeleteLog.h"

#include "engine/CdbOid.h"

#include "schema_table/record/table_record/CdbTableIdentifier.h"

#include "base_io/ByteBuffer.h"

#include "engine_lock/WriteLockHandle.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "engine_lock/StackDbLockUnlocker.h"

#include "trx/session/base/CdbDatabaseSession.h"
#include "trx/session/base/UndoSpaceManager.h"
#include "trx/session/base/UndoRegisterTransaction.h"

#include "trx/session/dml/UndoDeleteLog.h"

namespace codablecash {

DeleteLog::DeleteLog(uint64_t oid) : AbstractTransactionLog(AbstractTransactionLog::TRX_DELETE, oid) {
	this->table = nullptr;
}

DeleteLog::~DeleteLog() {
	delete this->table;
	this->oids.deleteElements();
}

int DeleteLog::binarySize() const {
	checkNotNull(this->table);

	int total = sizeof(uint8_t) + sizeof(uint64_t);

	total += this->table->binarySize();

	int maxLoop = this->oids.size();
	total += sizeof(int32_t);

	for(int i = 0; i != maxLoop; ++i){
		CdbOid* rec = this->oids.get(i);

		total += rec->binarySize();
	}

	return total;
}

void DeleteLog::toBinary(ByteBuffer* out) const {
	checkNotNull(this->table);

	out->put(AbstractTransactionLog::TRX_DELETE);
	out->putLong(this->oid);

	this->table->toBinary(out);

	int maxLoop = this->oids.size();
	out->putInt(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		CdbOid* rec = this->oids.get(i);

		rec->toBinary(out);
	}
}

void DeleteLog::fromBinary(ByteBuffer* in) {
	this->table = CdbTableIdentifier::fromBinary(in);

	int maxLoop = in->getInt();
	for(int i = 0; i != maxLoop; ++i){
		CdbOid* rec = CdbOid::fromBinary(in);
		addOid(rec);
	}
}

void DeleteLog::addOid(CdbOid* oid) noexcept {
	this->oids.addElement(oid);
}

void DeleteLog::commit(CdbTransactionManager* trxManager, CdbTransaction* trx) {
	CdbDatabaseSession* session = trxManager->getCdbDatabaseSession();
	UndoSpaceManager* undoManager = session->getUndoSpaceManager();

	{
		WriteLockHandle* lockH = trxManager->databaseWriteLock();
		StackDbLockUnlocker unlocker(lockH);

		// Undo Delete Log
		UndoRegisterTransaction undoTrx(undoManager);

		UndoDeleteLog log;
		log.setTableId(this->table);
		session->addUndoLog(&log);

		trxManager->commitDelete(this, trx, session);

		undoTrx.commit();
	}
}

} /* namespace codablecash */
