/*
 * InnerJoinExecutor.cpp
 *
 *  Created on: 2020/08/23
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner_join/InnerJoinExecutor.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "trx/scan/transaction_scanner_join/JoinCandidateCursor.h"

#include "scan_select/scan_planner/scanner/join/AbstractJoinCandidate.h"

#include "scan_select/scan_planner/scanner/ctx/ScanJoinContext.h"

#include "schema_table/record/table_record_key/AbstractCdbKey.h"

#include "base/StackRelease.h"

#include "trx/scan/transaction_scanner_join/IJoinRightSource.h"

#include "trx/scan/transaction_scanner_join_right/IRightOrTransactionScanner.h"
namespace codablecash {

InnerJoinExecutor::InnerJoinExecutor(IJoinLeftSource* left, IJoinRightSource* right, ScanResultMetadata* metadata
		, ScanJoinContext* context, AbstractScanCondition* filterCondition, LocalOidFactory* localOidFactory)
		: AbstractJoinExecutor(left, right, metadata, context, filterCondition, localOidFactory){
	this->leftRecord = nullptr;
	this->joinCandidateCursor = nullptr;
	this->nextRecord = nullptr;
}

InnerJoinExecutor::~InnerJoinExecutor() {
	shutdown();

	delete this->leftRecord;
	delete this->nextRecord;
	delete this->joinCandidateCursor;
}

void InnerJoinExecutor::start(VirtualMachine* vm) {
	AbstractJoinExecutor::start(vm);

	AbstractJoinCandidate* joinCandidate = this->context->getJoinCandidate();
	this->joinCandidateCursor = new JoinCandidateCursor(joinCandidate);
	this->joinCandidateCursor->init(vm, true);
}

bool InnerJoinExecutor::hasNext(VirtualMachine* vm) {
	while(!this->joinCandidateCursor->finished()){
		if(!hasNextLeftRecord(vm)){
			this->joinCandidateCursor->inc();

			IRightOrTransactionScanner* rightOrScanner = dynamic_cast<IRightOrTransactionScanner*>(this->right);
			if(rightOrScanner != nullptr){
				rightOrScanner->increaseOrCondition();
			}

			restartScan(vm);

			resetLeftRecord();
			continue;
		}

		// right scan for left record
		if(!this->right->hasNext(vm)){
			delete this->nextRecord, this->nextRecord = nullptr;

			resetLeftRecord();
			continue;
		}

		const CdbRecord* rightRecord = this->right->next(vm);

		delete this->nextRecord, this->nextRecord = nullptr;
		this->nextRecord = this->leftRecord->joinRecord(rightRecord);

		if(!checkByFilterCondition(vm, this->nextRecord) || this->joinCandidateCursor->hasAlreadyScanned(this->nextRecord)){
			continue;
		}

		setLocalOid(this->nextRecord);

		return true;
	}

	return false;
}

void InnerJoinExecutor::resetLeftRecord() {
	delete this->leftRecord;
	this->leftRecord = nullptr;
}

bool InnerJoinExecutor::hasNextLeftRecord(VirtualMachine* vm) {
	if(this->leftRecord == nullptr && this->left->hasNext(vm)){
		delete this->leftRecord, this->leftRecord = nullptr;

		const CdbRecord* rec = this->left->next(vm);

		AbstractCdbValue* v = rec->copy();
		this->leftRecord = dynamic_cast<CdbRecord*>(v);
		onChangeLeft();
	}

	return this->leftRecord != nullptr;
}

void InnerJoinExecutor::onChangeLeft() {
	AbstractCdbKey* key = this->joinCandidateCursor->makeKey(this->leftRecord); __STP(key);

	this->right->reset(key);
}

const CdbRecord* InnerJoinExecutor::next(VirtualMachine* vm) {
	return this->nextRecord;
}

void InnerJoinExecutor::shutdown() {
	AbstractJoinExecutor::shutdown();
}

} /* namespace codablecash */
