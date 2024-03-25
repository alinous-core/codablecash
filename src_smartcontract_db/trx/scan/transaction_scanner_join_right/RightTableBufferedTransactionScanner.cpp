/*
 * RightTableTransactionScanner.cpp
 *
 *  Created on: 2020/12/19
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner_join_right/RightTableBufferedTransactionScanner.h"

#include "trx/scan/transaction_scanner/TableTransactionScanner.h"

#include "trx/transaction/CdbTransaction.h"
#include "trx/transaction/CdbTransactionManager.h"

#include "trx/transaction_cache/SingleKeyOidCache.h"
#include "trx/transaction_cache/OidKeyRecordCache.h"

#include "scan_select/scan_planner/scanner/join/AbstractJoinCandidate.h"
#include "scan_select/scan_planner/scanner/join/AbstractJoinCandidateCollection.h"

#include "base/StackRelease.h"

#include "engine/CodableDatabase.h"
#include "engine/CdbLocalCacheManager.h"
#include "engine/CdbOid.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/record/table_record_value/CdbOidValueList.h"

#include "schema_table/record/table_record_key/CdbOidKey.h"


namespace codablecash {

RightTableBufferedTransactionScanner::RightTableBufferedTransactionScanner(
		ScanResultMetadata* metadata, CdbTransaction* trx,
		const CdbTable* table, const AbstractScanCondition* filterCondition, AbstractJoinCandidate* joinCandidate, bool leftReverse)
	: AbstractRightTransactionScanner(metadata, trx, table, filterCondition) {
	this->joinCandidate = dynamic_cast<AbstractJoinCandidateCollection*>(joinCandidate->copy());
	this->leftReverse = leftReverse;

	this->keyOidCache = nullptr;
	this->oidRecordCache = nullptr;

	this->data = nullptr;
	this->current = 0;
}

RightTableBufferedTransactionScanner::~RightTableBufferedTransactionScanner() {
	shutdown();
	this->data = nullptr;
	delete this->joinCandidate;
}

void RightTableBufferedTransactionScanner::start(VirtualMachine* vm) {
	CdbTransactionManager* trxManager = this->trx->getTrxManager();
	CodableDatabase* db = trxManager->getDb();

	CdbLocalCacheManager* cacheManager = db->getLocalCacheManager();

	this->keyOidCache = cacheManager->createSingleKeyOidCache();
	this->oidRecordCache = cacheManager->createOidKeyRecordCache();

	TableTransactionScanner* scanner = this->trx->getTableTransactionScanner(this->table, this->metadata, this->filterCondition); __STP(scanner);
	scanner->start(vm);

	while(scanner->hasNext(vm)){
		const CdbRecord* rec = scanner->next(vm);
		AbstractCdbKey* key = this->joinCandidate->makeKeyFromRecord(rec, this->leftReverse); __STP(key);

		const CdbOid* oid = rec->getOid();


		CdbRecord* scannedRecord = new CdbRecord(*rec); __STP(scannedRecord);
		scannedRecord->initScanIdentity();

		this->keyOidCache->insert(key, oid);
		this->oidRecordCache->insert(scannedRecord);
	}
}

void RightTableBufferedTransactionScanner::restartScan(VirtualMachine* vm) {
}

void RightTableBufferedTransactionScanner::reset(const AbstractCdbKey* key) {
	this->data = this->keyOidCache->findOids(key);
	this->current = 0;
}

bool RightTableBufferedTransactionScanner::hasNext(VirtualMachine* vm) {
	return this->data != nullptr && this->current < this->data->size();
}

const CdbRecord* RightTableBufferedTransactionScanner::next(VirtualMachine* vm) {
	CdbOid* oid = this->data->get(this->current++);
	CdbOidKey* oidKey = dynamic_cast<CdbOidKey*>(oid->toKey()); __STP(oidKey);

	const CdbRecord* rec = this->oidRecordCache->findRecord(oidKey);

	return rec;
}

void RightTableBufferedTransactionScanner::shutdown() {
	delete this->keyOidCache;
	this->keyOidCache = nullptr;

	delete this->oidRecordCache;
	this->oidRecordCache = nullptr;
}

} /* namespace codablecash */
