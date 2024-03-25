/*
 * JoinCandidateCursor.cpp
 *
 *  Created on: 2021/01/23
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner_join/JoinCandidateCursor.h"

#include "scan_select/scan_planner/scanner/join/AbstractJoinCandidateCollection.h"
#include "scan_select/scan_planner/scanner/join/AbstractJoinCandidate.h"
#include "scan_select/scan_planner/scanner/join/JoinOrCandidate.h"

#include "vm/VirtualMachine.h"

#include "engine/CodableDatabase.h"
#include "engine/CdbLocalCacheManager.h"

#include "trx/transaction_cache/RecordScanIdentityCache.h"

#include "schema_table/record/table_record/CdbRecord.h"
namespace codablecash {

JoinCandidateCursor::JoinCandidateCursor(AbstractJoinCandidate* joinCandidat) {
	this->joinCandidate = joinCandidat;
	this->pos = 0;
	this->cache = nullptr;
}

JoinCandidateCursor::~JoinCandidateCursor() {
	this->joinCandidate = nullptr;
	delete this->cache;
}

void JoinCandidateCursor::init(VirtualMachine* vm, bool isleft) noexcept {
	AbstractJoinCandidate::CandidateType candidateType = this->joinCandidate->getCandidateType();

	if(candidateType == AbstractJoinCandidate::CandidateType::OR){
		JoinOrCandidate* orCandidate = dynamic_cast<JoinOrCandidate*>(this->joinCandidate);

		int maxLoop = orCandidate->size();
		for(int i = 0; i != maxLoop; ++i){
			const AbstractJoinCandidateCollection* col = orCandidate->get(i);
			this->list.addElement(col);
		}
	}
	else {
		const AbstractJoinCandidateCollection* col = dynamic_cast<const AbstractJoinCandidateCollection*>(this->joinCandidate);
		this->list.addElement(col);
	}

	if(!isleft || !hasOrConditions()){
		return;
	}

	CodableDatabase* db = vm->getDb();
	CdbLocalCacheManager* cacheManager = db->getLocalCacheManager();

	this->cache = cacheManager->createRecordScanIdentityCache();
}

bool JoinCandidateCursor::finished() const noexcept {
	return !(this->pos < this->list.size());
}

void JoinCandidateCursor::inc() noexcept {
	this->pos++;
}

void JoinCandidateCursor::resetPos() noexcept {
	this->pos = 0;
}

AbstractCdbKey* JoinCandidateCursor::makeKey(const CdbRecord* leftRecord) noexcept {
	const AbstractJoinCandidateCollection* jcc = this->list.get(this->pos);

	return jcc->makeKeyFromRecord(leftRecord, true);
}

AbstractCdbKey* JoinCandidateCursor::makeKeyRight(const CdbRecord* rightRecord) noexcept {
	const AbstractJoinCandidateCollection* jcc = this->list.get(this->pos);

	return jcc->makeKeyFromRecord(rightRecord, false);
}

bool JoinCandidateCursor::hasOrConditions() const noexcept {
	return this->list.size() > 1;
}

bool JoinCandidateCursor::hasAlreadyScanned(const CdbRecord* record) {
	if(!hasOrConditions()){
		return false;
	}

	assert(this->cache != nullptr);

	ScanRecordIndentity* scanId = record->getScanIdentity();
	bool bl = this->cache->hasKey(scanId);

	if(!bl){
		this->cache->insert(scanId);
	}

	// hasAlreadyScanned
	return bl;
}

} /* namespace codablecash */
