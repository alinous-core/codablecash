/*
 * InsertLog.cpp
 *
 *  Created on: 2020/05/28
 *      Author: iizuka
 */

#include "trx/transaction_log/InsertLog.h"

#include "base_io/ByteBuffer.h"

#include "schema_table/record/table_record/CdbRecord.h"
#include "schema_table/record/table_record/CdbTableIdentifier.h"

#include "trx/transaction/CdbTransactionManager.h"
#include "trx/transaction/RecordObjectIdPublisher.h"
#include "trx/transaction/CdbTransaction.h"
#include "trx/transaction/LocalOid2PublishedOidLog.h"

#include "base/StackRelease.h"

#include "engine/CdbOid.h"

#include "engine_lock/WriteLockHandle.h"
#include "engine_lock/ReadLockHandle.h"
#include "engine_lock/StackDbLockUnlocker.h"

#include "trx/session/base/CdbDatabaseSession.h"

#include "trx/session/base/UndoRegisterTransaction.h"

#include "trx/session/dml/UndoInsertLog.h"

#include "trx/session/record/UndoInsertRecordPart.h"
namespace codablecash {

InsertLog::InsertLog(uint64_t oid) : AbstractTransactionLog(AbstractTransactionLog::TRX_INSERT, oid) {
	this->table = nullptr;
}

InsertLog::~InsertLog() {
	delete this->table;
	this->records.deleteElements();
}

int InsertLog::binarySize() const {
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

void InsertLog::toBinary(ByteBuffer* out) const {
	checkNotNull(this->table);

	out->put(AbstractTransactionLog::TRX_INSERT);
	out->putLong(this->oid);

	this->table->toBinary(out);

	int maxLoop = this->records.size();
	out->putInt(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		CdbRecord* rec = this->records.get(i);

		rec->toBinary(out);
	}
}

void InsertLog::fromBinary(ByteBuffer* in) {
	this->table = CdbTableIdentifier::fromBinary(in);

	int maxLoop = in->getInt();
	for(int i = 0; i != maxLoop; ++i){
		CdbRecord* rec = CdbRecord::createFromBinary(in);
		addRecord(rec);
	}
}

void InsertLog::commit(CdbTransactionManager* trxManager, CdbTransaction* trx) {
	CdbDatabaseSession* session = trxManager->getCdbDatabaseSession();
	UndoSpaceManager* undoManager = session->getUndoSpaceManager();

	{
		WriteLockHandle* lockH = trxManager->databaseWriteLock();
		StackDbLockUnlocker unlocker(lockH);

		RecordObjectIdPublisher* publisher = trxManager->getRecordObjectIdPublisher();
		LocalOid2PublishedOidLog* oidLog = trx->getOidPublishLog();

		// Undo Insert Log
		UndoRegisterTransaction undoTrx(undoManager);
		{
			UndoInsertLog log;
			log.setTableId(this->table);

			session->addUndoLog(&log);
		}

		int maxLoop = this->records.size();
		for(int i = 0; i != maxLoop; ++i){
			CdbRecord* rec = this->records.get(i);
			uint64_t oid = publisher->newOid();

			const CdbOid* lastLocalOid = rec->getOid();
			assert(lastLocalOid->isLocal());

			CdbOid oidObj(oid);

			oidLog->addLocalOid2Oid(lastLocalOid, &oidObj);

			rec->setOid(&oidObj);

			// undo log part
			UndoInsertRecordPart undoPart;
			undoPart.setRecordOid(oid);
			session->addUndoLogPart(&undoPart);
		}

		publisher->saveSchema();

		trxManager->commitInsert(this);

		undoTrx.commit();
	}
}

void InsertLog::addRecord(CdbRecord* record) noexcept {
	record->clearScanIdentity();
	this->records.addElement(record);
}

void InsertLog::setTable(CdbTableIdentifier* table) noexcept {
	this->table = table;
}

const ArrayList<CdbRecord>* InsertLog::getRecords() const noexcept {
	return &this->records;
}


} /* namespace codablecash */
