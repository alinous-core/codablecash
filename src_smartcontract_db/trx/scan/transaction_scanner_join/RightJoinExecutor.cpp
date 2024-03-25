/*
 * RightJoinExecutor.cpp
 *
 *  Created on: 2021/12/28
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner_join/RightJoinExecutor.h"
#include "trx/scan/transaction_scanner_join/JoinCandidateCursor.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "scan_select/scan_planner/scanner/join/AbstractJoinCandidate.h"

#include "scan_select/scan_planner/scanner/ctx/ScanJoinContext.h"

#include "base/StackRelease.h"

#include "trx/scan/transaction_scanner_join/IJoinRightSource.h"

#include "schema_table/record/table_record_key/AbstractCdbKey.h"

#include "base/ArrayList.h"

#include "trx/scan/transaction_scan_result/ScanResultFieldMetadata.h"
#include "trx/scan/transaction_scan_result/ScanResultMetadata.h"

#include "scan_select/scan_condition/exp_escape/EscapeTargetCondittionSetter.h"

#include "trx/scan/transaction_scanner_join_right/IRightOrTransactionScanner.h"
namespace codablecash {

/**
 * codablecash::RightJoinScannerFactory
 */
RightJoinExecutor::RightJoinExecutor(IJoinLeftSource* left, IJoinRightSource* right, ScanResultMetadata* metadata, ScanJoinContext* context
		, AbstractScanCondition* filterCondition, LocalOidFactory* localOidFactory)
				: AbstractJoinExecutor(left, right, metadata, context, filterCondition, localOidFactory){
	this->rightRecord = nullptr;
	this->joinCandidateCursor = nullptr;
	this->nextRecord = nullptr;

	this->joined = false;
}

RightJoinExecutor::~RightJoinExecutor() {
	shutdown();

	delete this->rightRecord;
	delete this->nextRecord;
	delete this->joinCandidateCursor;
}

void RightJoinExecutor::start(VirtualMachine* vm) {
	AbstractJoinExecutor::start(vm);

	AbstractJoinCandidate* joinCandidate = this->context->getJoinCandidate();
	this->joinCandidateCursor = new JoinCandidateCursor(joinCandidate);
	this->joinCandidateCursor->init(vm, true);
}

bool RightJoinExecutor::hasNext(VirtualMachine* vm) {
	AbstractJoinCandidate* joinCandidate = this->context->getJoinCandidate();
	const AbstractScanTableTarget* rTarget = joinCandidate->getLeftTarget();

	// left is right
	while(!this->joinCandidateCursor->finished()){
		if(!hasNextRightRecord(vm)){
			this->joinCandidateCursor->inc();

			IRightOrTransactionScanner* rightOrScanner = dynamic_cast<IRightOrTransactionScanner*>(this->right);
			if(rightOrScanner != nullptr){
				rightOrScanner->increaseOrCondition();
			}

			restartScan(vm);

			resetRightRecord();
			this->joined = false;
			continue;
		}

		// right scan for left record
		if(!this->right->hasNext(vm)){
			delete this->nextRecord, this->nextRecord = nullptr;

			if(!this->joined){
				const ScanResultMetadata* leftMeta = this->right->getMetadata(); // right scan for left source
				const ArrayList<ScanResultFieldMetadata>* l = leftMeta->getList();

				// add null
				int size = l->size();
				delete this->nextRecord, this->nextRecord = nullptr;

				CdbRecord* newRecord = new CdbRecord(); __STP(newRecord);
				newRecord->initNullColumns(size);
				newRecord->initBlankScanIdentity();
				this->nextRecord = newRecord->joinRecord(this->rightRecord);

				{
					EscapeTargetCondittionSetter escapeSetter(vm, rTarget);
					if(!checkByFilterCondition(vm, this->nextRecord) || this->joinCandidateCursor->hasAlreadyScanned(this->nextRecord)){
						this->joined = false;
						resetRightRecord();
						continue;
					}
				}

				setLocalOid(this->nextRecord);

				this->joined = false;

				resetRightRecord();

				return true;
			}

			// register right null as scanned
			this->joinCandidateCursor->hasAlreadyScanned(this->rightRecord);

			this->joined = false;
			resetRightRecord();

			continue;
		}

		const CdbRecord* leftRecord = this->right->next(vm); // leftRecord is right

		delete this->nextRecord, this->nextRecord = nullptr;
		this->nextRecord = leftRecord->joinRecord(this->rightRecord); // this->rightRecord->joinRecord(leftRecord);

		if(!checkByFilterCondition(vm, this->nextRecord) || this->joinCandidateCursor->hasAlreadyScanned(this->nextRecord)){
			continue;
		}

		setLocalOid(this->nextRecord);

		this->joined = true;

		return true;
	}

	return false;
}

void RightJoinExecutor::resetRightRecord() {
	delete this->rightRecord;
	this->rightRecord = nullptr;
}

bool RightJoinExecutor::hasNextRightRecord(VirtualMachine* vm) {
	// left is right
	if(this->rightRecord == nullptr && this->left->hasNext(vm)){
		delete this->rightRecord, this->rightRecord = nullptr;

		const CdbRecord* rec = this->left->next(vm);

		AbstractCdbValue* v = rec->copy();
		this->rightRecord = dynamic_cast<CdbRecord*>(v);
		onChangeRight();
	}

	return this->rightRecord != nullptr;
}

void RightJoinExecutor::onChangeRight() {
	AbstractCdbKey* key = this->joinCandidateCursor->makeKeyRight(this->rightRecord); __STP(key);

	this->right->reset(key);
	this->joined = false;
}

const CdbRecord* RightJoinExecutor::next(VirtualMachine* vm) {
	return this->nextRecord;
}

void RightJoinExecutor::shutdown() {
	AbstractJoinExecutor::shutdown();
}

} /* namespace codablecash */
