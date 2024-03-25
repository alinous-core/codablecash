/*
 * OuterJoinExecutor.cpp
 *
 *  Created on: 2020/08/23
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner_join/OuterJoinExecutor.h"
#include "trx/scan/transaction_scanner_join/IJoinRightSource.h"
#include "trx/scan/transaction_scanner_join/JoinCandidateCursor.h"

#include "trx/scan/transaction_scanner_join_right/IRightOrTransactionScanner.h"

#include "schema_table/record/table_record/CdbRecord.h"
#include "schema_table/record/table_record_key/AbstractCdbKey.h"

#include "scan_select/scan_planner/scanner/join/AbstractJoinCandidate.h"

#include "scan_select/scan_planner/scanner/ctx/ScanJoinContext.h"

#include "base/StackRelease.h"

#include "base/ArrayList.h"

#include "trx/scan/transaction_scan_result/ScanResultFieldMetadata.h"
#include "trx/scan/transaction_scan_result/ScanResultMetadata.h"

#include "scan_select/scan_condition/exp_escape/EscapeTargetCondittionSetter.h"


using namespace alinous;

namespace codablecash {

OuterJoinExecutor::OuterJoinExecutor(IJoinLeftSource* left, IJoinRightSource* right, ScanResultMetadata* metadata
		, ScanJoinContext* context, AbstractScanCondition* filterCondition, LocalOidFactory* localOidFactory)
					: AbstractJoinExecutor(left, right, metadata, context, filterCondition, localOidFactory){
	this->leftRecord = nullptr;
	this->joinCandidateCursor = nullptr;
	this->nextRecord = nullptr;

	this->joined = false;
}

OuterJoinExecutor::~OuterJoinExecutor() {
	shutdown();

	delete this->leftRecord;
	delete this->nextRecord;
	delete this->joinCandidateCursor;
}

void OuterJoinExecutor::start(VirtualMachine* vm) {
	AbstractJoinExecutor::start(vm);

	AbstractJoinCandidate* joinCandidate = this->context->getJoinCandidate();
	this->joinCandidateCursor = new JoinCandidateCursor(joinCandidate);
	this->joinCandidateCursor->init(vm, true);
}

bool OuterJoinExecutor::hasNext(VirtualMachine* vm) {
	AbstractJoinCandidate* joinCandidate = this->context->getJoinCandidate();
	const AbstractScanTableTarget* rTarget = joinCandidate->getRightTarget();

	while(!this->joinCandidateCursor->finished()){
		if(!hasNextLeftRecord(vm)){
			this->joinCandidateCursor->inc();

			IRightOrTransactionScanner* rightOrScanner = dynamic_cast<IRightOrTransactionScanner*>(this->right);
			if(rightOrScanner != nullptr){
				rightOrScanner->increaseOrCondition();
			}

			restartScan(vm);

			resetLeftRecord();
			this->joined = false;
			continue;
		}

		// right scan for left record
		if(!this->right->hasNext(vm)){
			delete this->nextRecord, this->nextRecord = nullptr;

			if(!this->joined){
				const ScanResultMetadata* rightMeta = this->right->getMetadata();
				const ArrayList<ScanResultFieldMetadata>* l = rightMeta->getList();

				// add null
				int size = l->size();
				delete this->nextRecord, this->nextRecord = nullptr;
				this->nextRecord = this->leftRecord->joinEmptyRecord(size);

				{
					EscapeTargetCondittionSetter escapeSetter(vm, rTarget);
					if(!checkByFilterCondition(vm, this->nextRecord) || this->joinCandidateCursor->hasAlreadyScanned(this->nextRecord)){
						this->joined = false;
						resetLeftRecord();
						continue;
					}
				}

				setLocalOid(this->nextRecord);

				this->joined = false;

				resetLeftRecord();

				return true;
			}

			// register right null as scanned
			this->joinCandidateCursor->hasAlreadyScanned(this->leftRecord);

			this->joined = false;
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

		this->joined = true;

		return true;
	}

	return false;
}

void OuterJoinExecutor::resetLeftRecord() {
	delete this->leftRecord;
	this->leftRecord = nullptr;
}

bool OuterJoinExecutor::hasNextLeftRecord(VirtualMachine* vm) {
	if(this->leftRecord == nullptr && this->left->hasNext(vm)){
		delete this->leftRecord, this->leftRecord = nullptr;

		const CdbRecord* rec = this->left->next(vm);

		AbstractCdbValue* v = rec->copy();
		this->leftRecord = dynamic_cast<CdbRecord*>(v);
		onChangeLeft();
	}

	return this->leftRecord != nullptr;
}

void OuterJoinExecutor::onChangeLeft() {
	AbstractCdbKey* key = this->joinCandidateCursor->makeKey(this->leftRecord); __STP(key);

	this->right->reset(key);
	this->joined = false;
}

const CdbRecord* OuterJoinExecutor::next(VirtualMachine* vm) {
	return this->nextRecord;
}

void OuterJoinExecutor::shutdown() {
	AbstractJoinExecutor::shutdown();
}

} /* namespace codablecash */
