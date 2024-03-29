/*
 * JoinMuntipleCandidate.cpp
 *
 *  Created on: 2020/08/31
 *      Author: iizuka
 */

#include "scan_select/scan_planner/scanner/join/JoinMultipleCandidate.h"
#include "scan_select/scan_planner/scanner/join/JoinCandidate.h"
#include "scan_select/scan_planner/scanner/join/JoinOrCandidate.h"

#include "base/ArrayList.h"

#include "engine/CdbOid.h"

#include "schema_table/table/CdbTableColumn.h"
#include "schema_table/table/CdbTable.h"

#include "scan_select/scan_table/TableScanTarget.h"

#include "schema_table/record/table_record_key/CdbRecordKey.h"
#include "schema_table/record/table_record_key/AbstractCdbKey.h"

#include "schema_table/record/table_record/CdbRecord.h"


namespace codablecash {

codablecash::JoinMultipleCandidate::JoinMultipleCandidate(const JoinMultipleCandidate& inst) : AbstractJoinCandidateCollection(joinType) {
	int maxLoop = inst.list.size();
	for(int i = 0; i != maxLoop; ++i){
		const JoinCandidate* candidate = inst.list.get(i);

		this->list.addElement(new JoinCandidate(*candidate));
	}
}

JoinMultipleCandidate::JoinMultipleCandidate(int joinType) : AbstractJoinCandidateCollection(joinType) {
}

JoinMultipleCandidate::~JoinMultipleCandidate() {
	this->list.deleteElements();
}

AbstractJoinCandidate::CandidateType JoinMultipleCandidate::getCandidateType() const noexcept {
	return AbstractJoinCandidate::CandidateType::AND;
}

const AbstractScanTableTarget* JoinMultipleCandidate::getLeftTarget() const noexcept {
	const JoinCandidate* c = get(0);

	return c->getLeftTarget();
}

const AbstractScanTableTarget* JoinMultipleCandidate::getRightTarget() const noexcept {
	const JoinCandidate* c = get(0);

	return c->getRightTarget();
}

AbstractJoinCandidate* JoinMultipleCandidate::multiply(const AbstractJoinCandidate* other) const noexcept {
	JoinCandidate::CandidateType candidateType = other->getCandidateType();

	if(candidateType == JoinCandidate::CandidateType::OR){
		const JoinOrCandidate* orCandidate = dynamic_cast<const JoinOrCandidate*>(other);
		return orCandidate->multiply(this);
	}

	JoinMultipleCandidate* candidate = new JoinMultipleCandidate(this->joinType);

	// copy self
	int maxLoop = size();
	for(int i = 0; i != maxLoop; ++i){
		const JoinCandidate* c = get(i);
		candidate->mul(c);
	}

	// other
	const AbstractJoinCandidateCollection* col = dynamic_cast<const AbstractJoinCandidateCollection*>(other);

	maxLoop = col->size();
	for(int i = 0; i != maxLoop; ++i){
		const JoinCandidate* c = col->get(i);
		candidate->mul(c);
	}

	return candidate;
}

int JoinMultipleCandidate::size() const noexcept {
	return this->list.size();
}

const JoinCandidate* JoinMultipleCandidate::get(int i) const noexcept {
	return this->list.get(i);
}

AbstractJoinCandidate* JoinMultipleCandidate::copy() const noexcept {
	return new JoinMultipleCandidate(*this);
}

void JoinMultipleCandidate::mul(const JoinCandidate* candidate) noexcept {
	this->list.addElement(new JoinCandidate(*candidate));
}

int JoinMultipleCandidate::getOverHeadScore(AbstractScanTableTarget* left, AbstractScanTableTarget* right) const noexcept {
	int score = 1000;

	CdbTableIndex* index = getIndex(right);
	if(index != nullptr){
		score = 1;
	}

	return score;
}


CdbTableIndex* JoinMultipleCandidate::getIndex(const AbstractScanTableTarget* right) const noexcept {
	CdbTableIndex* index = nullptr;
	ArrayList<const CdbOid> oidlist;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		const JoinCandidate* c = this->list.get(i);

		const CdbTableColumn* column = c->getRightColumn(right);
		if(column != nullptr){
			const CdbOid* columnOid = column->getOid();
			oidlist.addElement(columnOid);
		}
	}

	const TableScanTarget* tableTarget = dynamic_cast<const TableScanTarget*>(right);
	if(tableTarget != nullptr){
		const CdbTable* table = tableTarget->getTable();
		index = !oidlist.isEmpty() ? table->findMostAvailableIndex(&oidlist) : nullptr;
	}

	return index;
}

AbstractCdbKey* JoinMultipleCandidate::makeKeyFromRecord(const CdbRecord* leftRecord, bool bleft) const noexcept {
	CdbRecordKey* retkey = new CdbRecordKey();

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		const JoinCandidate* c = this->list.get(i);

		AbstractCdbKey* abkey = c->makeKeyFromRecord(leftRecord, bleft);
		retkey->addKey(abkey);
	}

	return retkey;
}

} /* namespace codablecash */
