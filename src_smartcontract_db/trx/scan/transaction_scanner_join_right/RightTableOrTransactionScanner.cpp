/*
 * RightTableOrTransactionScanner.cpp
 *
 *  Created on: 2020/12/20
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner_join_right/RightTableOrTransactionScanner.h"

#include "scan_select/scan_planner/scanner/join/JoinOrCandidate.h"

#include "trx/scan/transaction_scanner_join/JoinCandidateCursor.h"

namespace codablecash {

RightTableOrTransactionScanner::RightTableOrTransactionScanner(ScanResultMetadata* metadata, CdbTransaction* trx,
		const CdbTable* table, const AbstractScanCondition* filterCondition, const JoinOrCandidate* orCandidate)
		: AbstractRightTransactionScanner(metadata, trx, table, filterCondition){
	this->orCandidate = new JoinOrCandidate(*orCandidate);
	this->joinCandidateCursor = nullptr;
	this->currentSource = nullptr;
}

RightTableOrTransactionScanner::~RightTableOrTransactionScanner() {
	shutdown();

	delete this->orCandidate;

	delete this->joinCandidateCursor;
	this->currentSource = nullptr;
}

void RightTableOrTransactionScanner::addRightSource(IJoinRightSource* joinSrc) noexcept {
	this->srclist.addElement(joinSrc);
}

void RightTableOrTransactionScanner::start(VirtualMachine* vm) {
	this->joinCandidateCursor = new JoinCandidateCursor(this->orCandidate);
	this->joinCandidateCursor->init(vm, false);

	int maxLoop = this->srclist.size();
	for(int i = 0; i != maxLoop; ++i){
		IJoinRightSource* src = this->srclist.get(i);

		src->start(vm);
	}
}

void RightTableOrTransactionScanner::restartScan(VirtualMachine* vm) {
	AbstractRightTransactionScanner::restartScan(vm);

	this->joinCandidateCursor->resetPos();

	int maxLoop = this->srclist.size();
	for(int i = 0; i != maxLoop; ++i){
		IJoinRightSource* src = this->srclist.get(i);

		src->restartScan(vm);
	}
}

void RightTableOrTransactionScanner::reset(const AbstractCdbKey* key) {
	int pos = this->joinCandidateCursor->getPos();
	this->currentSource = this->srclist.get(pos);

	this->currentSource->reset(key);
}

bool RightTableOrTransactionScanner::hasNext(VirtualMachine* vm) {
	return this->currentSource->hasNext(vm);
}

const CdbRecord* RightTableOrTransactionScanner::next(VirtualMachine* vm) {
	return this->currentSource->next(vm);
}

void RightTableOrTransactionScanner::shutdown() {
	if(!this->srclist.isEmpty()){
		int maxLoop = this->srclist.size();
		for(int i = 0; i != maxLoop; ++i){
			IJoinRightSource* src = this->srclist.get(i);

			src->shutdown();
		}

		this->srclist.deleteElements();
		this->srclist.reset();
	}
}

void RightTableOrTransactionScanner::increaseOrCondition() {
	this->joinCandidateCursor->inc();
}

} /* namespace codablecash */
