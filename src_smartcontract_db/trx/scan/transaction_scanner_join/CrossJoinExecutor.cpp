/*
 * CrossJoinExecutor.cpp
 *
 *  Created on: 2022/01/02
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner_join/CrossJoinExecutor.h"

#include "scan_select/scan_planner/scanner/join/AbstractJoinCandidate.h"

#include "scan_select/scan_planner/scanner/ctx/ScanJoinContext.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "trx/scan/transaction_scanner_join/IJoinLeftSource.h"

#include "trx/scan/transaction_scanner_join/IJoinRightSource.h"
namespace codablecash {

CrossJoinExecutor::CrossJoinExecutor(IJoinLeftSource* left, IJoinRightSource* right, ScanResultMetadata* metadata, ScanJoinContext* context
		, AbstractScanCondition* filterCondition, LocalOidFactory* localOidFactory)
			: AbstractJoinExecutor(left, right, metadata, context, filterCondition, localOidFactory) {
	this->leftRecord = nullptr;
	this->nextRecord = nullptr;
}

CrossJoinExecutor::~CrossJoinExecutor() {
	shutdown();

	delete this->leftRecord;
	delete this->nextRecord;
}

void CrossJoinExecutor::start(VirtualMachine* vm) {
	AbstractJoinExecutor::start(vm);

}

bool CrossJoinExecutor::hasNext(VirtualMachine* vm) {
	while(hasNextLeftRecord(vm)){
		// right scan for left record
		if(!this->right->hasNext(vm)){
			delete this->nextRecord, this->nextRecord = nullptr;

			resetLeftRecord();
			continue;
		}

		const CdbRecord* rightRecord = this->right->next(vm);

		delete this->nextRecord, this->nextRecord = nullptr;
		this->nextRecord = this->leftRecord->joinRecord(rightRecord);

		if(!checkByFilterCondition(vm, this->nextRecord)){
			continue;
		}

		setLocalOid(this->nextRecord);

		return true;
	}

	return false;
}

void CrossJoinExecutor::resetLeftRecord() {
	delete this->leftRecord;
	this->leftRecord = nullptr;
}

bool CrossJoinExecutor::hasNextLeftRecord(VirtualMachine* vm) {
	if(this->leftRecord == nullptr && this->left->hasNext(vm)){
		delete this->leftRecord, this->leftRecord = nullptr;

		const CdbRecord* rec = this->left->next(vm);

		AbstractCdbValue* v = rec->copy();
		this->leftRecord = dynamic_cast<CdbRecord*>(v);
		onChangeLeft();
	}

	return this->leftRecord != nullptr;
}

void CrossJoinExecutor::onChangeLeft() {
	this->right->reset(nullptr);
}


const CdbRecord* CrossJoinExecutor::next(VirtualMachine* vm) {
	return this->nextRecord;
}

void CrossJoinExecutor::shutdown() {
	AbstractJoinExecutor::shutdown();
}

} /* namespace codablecash */
