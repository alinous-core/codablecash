/*
 * IndexResolver.cpp
 *
 *  Created on: 2020/11/26
 *      Author: iizuka
 */

#include "scan_select/scan_planner/scanner/index_resolv/IndexResolver.h"
#include "scan_select/scan_planner/scanner/index/AbstractIndexCandidate.h"
#include "scan_select/scan_planner/scanner/index/OrIndexCandidate.h"
#include "scan_select/scan_planner/scanner/index/MultipleIndexCandidate.h"
#include "scan_select/scan_planner/scanner/index/IndexRangeCandidate.h"

#include "engine/CodableDatabase.h"

#include "scan_select/scan_condition/params/ColumnIdentifierScanParam.h"

#include "base/StackRelease.h"
#include "scan_select/scan_planner/scanner/index_resolv/AbstractColumnsIndexAccess.h"

#include "scan_select/scan_planner/scanner/index_resolv/MultipleColumnIndexAccess.h"
#include "scan_select/scan_planner/scanner/index_resolv/OrIndexAccessCollection.h"
#include "scan_select/scan_planner/scanner/index_resolv/SingleColumnIndexAccess.h"

namespace codablecash {

IndexResolver::IndexResolver(CodableDatabase* db) {
	this->db = db;
	this->result = nullptr;
}

IndexResolver::~IndexResolver() {
	this->db = nullptr;
	delete this->result;
}

void IndexResolver::analyze(const AbstractIndexCandidate* candidate, VirtualMachine* vm) {
	this->result = doAnalyze(candidate, vm);
}

AbstractColumnsIndexAccess* IndexResolver::doAnalyze(const AbstractIndexCandidate* candidate, VirtualMachine* vm) {
	SchemaManager* schemaManager = this->db->getSchemaManager();

	AbstractIndexCandidate::IndexType candidateType = candidate->getCandidateType();

	if(candidateType == AbstractIndexCandidate::IndexType::OR){
		const OrIndexCandidate* orCandidate = dynamic_cast<const OrIndexCandidate*>(candidate);

		OrIndexAccessCollection* orWarpper = analyzeOr(orCandidate, vm);
		StackRelease<OrIndexAccessCollection> stOrWrapper(orWarpper);

		if(orWarpper != nullptr && orWarpper->hasIndex(schemaManager, vm)){
			return stOrWrapper.move();
		}
		return nullptr;
	}
	else if(candidateType == AbstractIndexCandidate::IndexType::AND){
		const MultipleIndexCandidate* andCandidate = dynamic_cast<const MultipleIndexCandidate*>(candidate);

		MultipleColumnIndexAccess* mindex = analyzeAnd(andCandidate, vm); __STP(mindex);

		return mindex->hasIndex(schemaManager, vm) ? __st_mindex__.move() : nullptr;
	}


	SingleColumnIndexAccess* index = handleSingleIndex(candidate, vm);
	StackRelease<SingleColumnIndexAccess> st_index(index);

	if(index != nullptr && index->hasIndex(schemaManager, vm)){
		return st_index.move();
	}

	return nullptr;
}

SingleColumnIndexAccess* IndexResolver::handleSingleIndex(const AbstractIndexCandidate* candidate, VirtualMachine* vm) {
	AbstractIndexCandidate::IndexType candidateType = candidate->getCandidateType();

	if(candidateType == AbstractIndexCandidate::IndexType::RANGE){
		const IndexRangeCandidate* rangeCandidate = dynamic_cast<const IndexRangeCandidate*>(candidate);

		const ColumnIdentifierScanParam* colp = rangeCandidate->getColumn();
		const CdbTableColumn* col = colp->getCdbColumn();

		SingleColumnIndexAccess* index = new SingleColumnIndexAccess();
		index->setRange(true, vm);
		index->setTarget(colp->getSourceTarget());
		index->setColumn(col);

		const IValueProvider* vp = rangeCandidate->getValue();
		index->setValue(vp);
		index->setBottomEq(rangeCandidate->isBottomEq());

		vp = rangeCandidate->getTopValue();
		index->setTopValue(vp);
		index->setTopEq(rangeCandidate->isTopEq());

		return index;
	}
	else if(candidateType == AbstractIndexCandidate::IndexType::RANGE_LT
			|| candidateType == AbstractIndexCandidate::IndexType::RANGE_LT_EQ){
		const IndexCandidate* indexCandidate = dynamic_cast<const IndexCandidate*>(candidate);
		const ColumnIdentifierScanParam* colp = indexCandidate->getColumn();
		const CdbTableColumn* col = colp->getCdbColumn();
		const IValueProvider* vp = indexCandidate->getValue();

		SingleColumnIndexAccess* index = new SingleColumnIndexAccess();
		index->setRange(true, vm);

		index->setTarget(colp->getSourceTarget());
		index->setColumn(col);

		index->setTopValue(vp);
		index->setTopEq(indexCandidate->hasEq());

		return index;
	}
	else if(candidateType == AbstractIndexCandidate::IndexType::RANGE_GT
			|| candidateType == AbstractIndexCandidate::IndexType::RANGE_GT_EQ){
		const IndexCandidate* indexCandidate = dynamic_cast<const IndexCandidate*>(candidate);
		const ColumnIdentifierScanParam* colp = indexCandidate->getColumn();
		const CdbTableColumn* col = colp->getCdbColumn();
		const IValueProvider* vp = indexCandidate->getValue();

		SingleColumnIndexAccess* index = new SingleColumnIndexAccess();
		index->setRange(true, vm);

		index->setTarget(colp->getSourceTarget());
		index->setColumn(col);

		index->setValue(vp);
		index->setBottomEq(indexCandidate->hasEq());

		return index;
	}

	const IndexCandidate* indexCandidate = dynamic_cast<const IndexCandidate*>(candidate);
	const ColumnIdentifierScanParam* colp = indexCandidate->getColumn();
	const CdbTableColumn* col = colp->getCdbColumn();
	const IValueProvider* vp = indexCandidate->getValue();

	SingleColumnIndexAccess* index = new SingleColumnIndexAccess();

	index->setTarget(colp->getSourceTarget());
	index->setColumn(col);
	index->setValue(vp);

	return index;
}

OrIndexAccessCollection* IndexResolver::analyzeOr(const OrIndexCandidate* orCandidate, VirtualMachine* vm) {
	OrIndexAccessCollection* wrapper = new OrIndexAccessCollection();

	AbstractIndexCandidateCollection* c = orCandidate->get(0);
	const IndexCandidate* ic = c->get(0);
	const ColumnIdentifierScanParam* colp = ic->getColumn();

	wrapper->setTarget(colp->getSourceTarget());

	int maxLoop = orCandidate->size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractIndexCandidateCollection* candidate = orCandidate->get(i);

		AbstractColumnsIndexAccess* w = handleIndexCollection(candidate, vm);
		wrapper->add(w);
	}

	return wrapper;
}

AbstractColumnsIndexAccess* IndexResolver::handleIndexCollection(const AbstractIndexCandidateCollection* candidate, VirtualMachine* vm) {
	AbstractIndexCandidate::IndexType candidateType = candidate->getCandidateType();

	if(candidateType == AbstractIndexCandidate::IndexType::AND){
		const MultipleIndexCandidate* andCandidate = dynamic_cast<const MultipleIndexCandidate*>(candidate);
		return analyzeAnd(andCandidate, vm);
	}

	return handleSingleIndex(candidate, vm);
}


MultipleColumnIndexAccess* IndexResolver::analyzeAnd(const MultipleIndexCandidate* andCandidate, VirtualMachine* vm) {
	MultipleColumnIndexAccess* wrapper = new MultipleColumnIndexAccess();

	const IndexCandidate* c = andCandidate->get(0);
	const IndexCandidate* ic = c->get(0);
	const ColumnIdentifierScanParam* colp = ic->getColumn();

	wrapper->setTarget(colp->getSourceTarget());

	int maxLoop = andCandidate->size();
	for(int i = 0; i != maxLoop; ++i){
		const IndexCandidate* candidate = andCandidate->get(i);

		SingleColumnIndexAccess* singleIndex = handleSingleIndex(candidate, vm);
		wrapper->add(singleIndex);
	}

	return wrapper;
}

} /* namespace codablecash */
