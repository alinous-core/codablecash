/*
 * MultipleIndexCandidate.cpp
 *
 *  Created on: 2020/09/07
 *      Author: iizuka
 */

#include "scan_select/scan_planner/scanner/index/MultipleIndexCandidate.h"
#include "scan_select/scan_planner/scanner/index/IndexCandidate.h"
#include "scan_select/scan_planner/scanner/index/OrIndexCandidate.h"

#include "scan_select/scan_planner/scanner/index/AbstractIndexCandidateCollection.h"
#include "scan_select/scan_planner/scanner/index/AbstractIndexCandidate.h"

#include "base/StackRelease.h"

#include "scan_select/scan_planner/scanner/index/IndexRangeCandidate.h"

#include "base/UnicodeString.h"

#include "scan_select/scan_condition/params/ColumnIdentifierScanParam.h"

#include "schema_table/table/CdbTableColumn.h"

#include "engine/CdbOid.h"
using namespace alinous;

namespace codablecash {

MultipleIndexCandidate::MultipleIndexCandidate(const MultipleIndexCandidate& inst) : AbstractIndexCandidateCollection(inst.indexType) {
	int maxLoop = inst.size();
	for(int i = 0; i != maxLoop; ++i){
		IndexCandidate* idx = inst.list.get(i);
		mul(idx);
	}
}

MultipleIndexCandidate::MultipleIndexCandidate() : AbstractIndexCandidateCollection(IndexType::AND) {

}

MultipleIndexCandidate::~MultipleIndexCandidate() {
	this->list.deleteElements();
}

AbstractIndexCandidate::IndexType MultipleIndexCandidate::getCandidateType() const noexcept {
	return AbstractIndexCandidate::IndexType::AND;
}

AbstractIndexCandidate* MultipleIndexCandidate::multiply(const AbstractIndexCandidate* other) const noexcept {
	AbstractIndexCandidate::IndexType candidateType = other->getCandidateType();

	if(candidateType == AbstractIndexCandidate::IndexType::OR){
		const OrIndexCandidate* orCandidate = dynamic_cast<const OrIndexCandidate*>(other);
		return orCandidate->multiply(this);
	}

	MultipleIndexCandidate* candidate = new MultipleIndexCandidate();

	// copy self
	int maxLoop = size();
	for(int i = 0; i != maxLoop; ++i){
		const IndexCandidate* c = get(i);
		candidate->mul(c);
	}

	// other
	AbstractIndexCandidateCollection* col = dynamic_cast<AbstractIndexCandidateCollection*>(other->copy());
	__STP(col);

	maxLoop = col->size();
	for(int i = 0; i != maxLoop; ++i){
		const IndexCandidate* c = col->get(i);
		candidate->mul(c);
	}

	return candidate;
}

AbstractIndexCandidate* MultipleIndexCandidate::copy() const noexcept {
	return new MultipleIndexCandidate(*this);
}

int MultipleIndexCandidate::size() const noexcept {
	return this->list.size();
}

const IndexCandidate* MultipleIndexCandidate::get(int i) const noexcept {
	return this->list.get(i);
}

void MultipleIndexCandidate::mul(const IndexCandidate* candidate) noexcept {
	addCandidate(candidate);
}

void MultipleIndexCandidate::addCandidate(const IndexCandidate* candidate) {
	if(candidate->isRange()){
		handleRangeCandidate(candidate);
		return;
	}

	// same column with equal
	if(hasSameColumn(candidate)){
		return;
	}

	this->list.addElement(new IndexCandidate(*candidate));
}

void MultipleIndexCandidate::handleRangeCandidate(const IndexCandidate* candidate) {
	bool used = false;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		IndexCandidate* idx = this->list.get(i);

		if(idx->isRangeJoinable(candidate)){
			IndexRangeCandidate* newCandidate = idx->toIndexRangeCandidate(candidate);
			assert(newCandidate != nullptr);

			this->list.setElement(newCandidate, i);
			delete idx;
			used = true;
			break;
		}
	}

	if(!used && !hasSameColumn(candidate)){
		this->list.addElement(new IndexCandidate(*candidate));
	}
}

bool MultipleIndexCandidate::hasSameColumn(const IndexCandidate *candidate) const noexcept {
	bool ret = false;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		IndexCandidate* idx = this->list.get(i);

		if(candidate->isSameColumn(idx)){
			ret = true;
			break;
		}
	}

	return ret;
}

const UnicodeString* MultipleIndexCandidate::toCodeString() noexcept {
	if(this->str == nullptr){
		this->str = new UnicodeString(L"");

		int maxLoop = this->list.size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractIndexCandidateCollection* col = this->list.get(i);

			if(i != 0){
				this->str->append(L" AND ");
			}

			const UnicodeString* colstr = col->toCodeString();
			this->str->append(colstr);
		}
	}

	return this->str;
}

} /* namespace codablecash */
