/*
 * RightWrappedLeftOrTransactionScanner.cpp
 *
 *  Created on: 2022/01/20
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner_join_right/RightWrappedLeftOrTransactionScanner.h"

#include "scan_select/scan_planner/scanner/join/JoinOrCandidate.h"
#include "scan_select/scan_planner/scanner/join/AbstractJoinCandidateCollection.h"

#include "trx/scan/transaction_scanner_join/JoinCandidateCursor.h"

#include "trx/transaction/CdbTransactionManager.h"
#include "trx/transaction/CdbTransaction.h"

#include "trx/transaction_cache/SingleKeyOidCache.h"

#include "schema_table/record/table_record_key/AbstractCdbKey.h"
#include "schema_table/record/table_record/CdbRecord.h"

#include "engine/CodableDatabase.h"
#include "engine/CdbLocalCacheManager.h"

#include "trx/transaction_cache/OidKeyRecordCache.h"

#include "base/StackRelease.h"

#include "schema_table/record/table_record_value/CdbOidValueList.h"

#include "schema_table/record/table_record_key/CdbOidKey.h"
namespace codablecash {

RightWrappedLeftOrTransactionScanner::RightWrappedLeftOrTransactionScanner(ScanResultMetadata* metadata, CdbTransaction* trx,
		const CdbTable* table, const AbstractScanCondition* filterCondition, IJoinLeftSource* leftSource, JoinOrCandidate* joinCandidate, bool leftReverse)
			: AbstractRightTransactionScanner(metadata, trx, table, filterCondition) {
	this->leftSource = leftSource;

	this->joinCandidate = dynamic_cast<JoinOrCandidate*>(joinCandidate->copy());
	this->leftReverse = leftReverse;

	this->joinCandidateCursor = nullptr;
	this->oidRecordCache = nullptr;

	this->data = nullptr;
	this->current = 0;
}

RightWrappedLeftOrTransactionScanner::~RightWrappedLeftOrTransactionScanner() {
	shutdown();

	delete this->leftSource;

	delete this->joinCandidate;
	delete this->joinCandidateCursor;
	this->data = nullptr;
}

void RightWrappedLeftOrTransactionScanner::start(VirtualMachine* vm) {
	CdbTransactionManager* trxManager = this->trx->getTrxManager();
	CodableDatabase* db = trxManager->getDb();

	CdbLocalCacheManager* cacheManager = db->getLocalCacheManager();

	this->joinCandidateCursor = new JoinCandidateCursor(this->joinCandidate);
	this->joinCandidateCursor->init(vm, false);

	// init keys list
	ArrayList<const AbstractJoinCandidateCollection>* list = this->joinCandidateCursor->getCandidatesList();
	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		const AbstractJoinCandidateCollection* candidate = list->get(i);

		SingleKeyOidCache* cache = cacheManager->createSingleKeyOidCache();
		this->caches.addElement(cache);
	}

	this->oidRecordCache = cacheManager->createOidKeyRecordCache();

	// scan
	this->leftSource->start(vm);
	while(this->leftSource->hasNext(vm)){
		const CdbRecord* record = this->leftSource->next(vm);

		makeCaches(record);

		this->oidRecordCache->insert(record);
	}
}

void RightWrappedLeftOrTransactionScanner::makeCaches(const CdbRecord* record) {
	ArrayList<const AbstractJoinCandidateCollection>* list = this->joinCandidateCursor->getCandidatesList();
	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		const AbstractJoinCandidateCollection* candidate = list->get(i);
		SingleKeyOidCache* cache = this->caches.get(i);

		AbstractCdbKey* key = candidate->makeKeyFromRecord(record, this->leftReverse); __STP(key);
		cache->insert(key, record->getOid());
	}
}

void RightWrappedLeftOrTransactionScanner::reset(const AbstractCdbKey* key) {
	int pos = this->joinCandidateCursor->getPos();

	SingleKeyOidCache* cache = this->caches.get(pos);
	this->data = cache->findOids(key);
	this->current = 0;
}

bool RightWrappedLeftOrTransactionScanner::hasNext(VirtualMachine* vm) {
	return this->data != nullptr && this->current < this->data->size();
}

const CdbRecord* RightWrappedLeftOrTransactionScanner::next(VirtualMachine* vm) {
	CdbOid* oid = this->data->get(this->current++);
	CdbOidKey* oidKey = dynamic_cast<CdbOidKey*>(oid->toKey()); __STP(oidKey);

	const CdbRecord* rec = this->oidRecordCache->findRecord(oidKey);

	return rec;
}

void RightWrappedLeftOrTransactionScanner::shutdown() {
	if(!this->caches.isEmpty()){
		this->caches.deleteElements();
		this->caches.reset();

		delete this->oidRecordCache;
	}
}

void RightWrappedLeftOrTransactionScanner::increaseOrCondition() {
	this->joinCandidateCursor->inc();
}

} /* namespace codablecash */
