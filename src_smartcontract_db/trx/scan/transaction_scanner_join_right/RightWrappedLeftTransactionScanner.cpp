/*
 * RightWrappedLeftTransactionScanner.cpp
 *
 *  Created on: 2022/01/05
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner_join_right/RightWrappedLeftTransactionScanner.h"

#include "trx/transaction/CdbTransaction.h"
#include "trx/transaction/CdbTransactionManager.h"

#include "trx/transaction_cache/SingleKeyOidCache.h"
#include "trx/transaction_cache/OidKeyRecordCache.h"

#include "trx/scan/transaction_scanner_join/IJoinLeftSource.h"

#include "scan_select/scan_planner/scanner/join/AbstractJoinCandidateCollection.h"

#include "engine/CdbLocalCacheManager.h"
#include "engine/CodableDatabase.h"

#include "base/StackRelease.h"

#include "schema_table/record/table_record_key/AbstractCdbKey.h"
#include "schema_table/record/table_record_key/CdbOidKey.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/record/table_record_value/CdbOidValueList.h"


namespace codablecash {

RightWrappedLeftTransactionScanner::RightWrappedLeftTransactionScanner(ScanResultMetadata* metadata, CdbTransaction* trx,
		const CdbTable* table, const AbstractScanCondition* filterCondition, IJoinLeftSource* leftSource, AbstractJoinCandidate* joinCandidate, bool leftReverse)
			: AbstractRightTransactionScanner(metadata, trx, table, filterCondition) {
	this->leftSource = leftSource;

	this->joinCandidate = dynamic_cast<AbstractJoinCandidateCollection*>(joinCandidate->copy());
	this->leftReverse = leftReverse;

	this->keyOidCache = nullptr;
	this->oidRecordCache = nullptr;

	this->data = nullptr;
	this->current = 0;
}

RightWrappedLeftTransactionScanner::~RightWrappedLeftTransactionScanner() {
	shutdown();

	delete this->leftSource;
	delete this->joinCandidate;
	this->data = nullptr;
}

void RightWrappedLeftTransactionScanner::start(VirtualMachine* vm) {
	CdbTransactionManager* trxManager = this->trx->getTrxManager();
	CodableDatabase* db = trxManager->getDb();

	CdbLocalCacheManager* cacheManager = db->getLocalCacheManager();

	this->keyOidCache = cacheManager->createSingleKeyOidCache();
	this->oidRecordCache = cacheManager->createOidKeyRecordCache();

	this->leftSource->start(vm);

	while(this->leftSource->hasNext(vm)){
		const CdbRecord* rec = this->leftSource->next(vm);
		AbstractCdbKey* key = this->joinCandidate->makeKeyFromRecord(rec, this->leftReverse); __STP(key);

		const CdbOid* oid = rec->getOid();

		this->keyOidCache->insert(key, oid);
		this->oidRecordCache->insert(rec);
	}
}

void RightWrappedLeftTransactionScanner::reset(const AbstractCdbKey* key) {
	this->data = this->keyOidCache->findOids(key);
	this->current = 0;
}

bool RightWrappedLeftTransactionScanner::hasNext(VirtualMachine* vm) {
	return this->data != nullptr && this->current < this->data->size();
}

const CdbRecord* RightWrappedLeftTransactionScanner::next(VirtualMachine* vm) {
	CdbOid* oid = this->data->get(this->current++);
	CdbOidKey* oidKey = dynamic_cast<CdbOidKey*>(oid->toKey()); __STP(oidKey);

	const CdbRecord* rec = this->oidRecordCache->findRecord(oidKey);

	return rec;
}

void RightWrappedLeftTransactionScanner::shutdown() {
	delete this->keyOidCache;
	this->keyOidCache = nullptr;

	delete this->oidRecordCache;
	this->oidRecordCache = nullptr;
}

} /* namespace codablecash */
