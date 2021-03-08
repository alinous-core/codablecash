/*
 * OuterJoinExecutor.cpp
 *
 *  Created on: 2020/08/23
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner_join/OuterJoinExecutor.h"
#include "trx/scan/transaction_scanner_join/IJoinRightSource.h"
#include "trx/scan/transaction_scanner_join/JoinCandidateCursor.h"

#include "schema_table/record/table_record/CdbRecord.h"
#include "schema_table/record/table_record_key/AbstractCdbKey.h"

#include "scan_select/scan_planner/scanner/join/AbstractJoinCandidate.h"

#include "scan_select/scan_planner/scanner/ctx/ScanJoinContext.h"

#include "base/StackRelease.h"

#include "base/ArrayList.h"

#include "trx/scan/transaction_scan_result/ScanResultFieldMetadata.h"
#include "trx/scan/transaction_scan_result/ScanResultMetadata.h"

using namespace alinous;

namespace codablecash {

OuterJoinExecutor::OuterJoinExecutor(IJoinLeftSource* left, IJoinRightSource* right, ScanResultMetadata* metadata
		, ScanJoinContext* context, AbstractScanCondition* filterCondition)
					: AbstractJoinExecutor(left, right, metadata, context, filterCondition){
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

void OuterJoinExecutor::start() {
	AbstractJoinExecutor::start();

	AbstractJoinCandidate* joinCandidate = this->context->getJoinCandidate();
	this->joinCandidateCursor = new JoinCandidateCursor(joinCandidate);
	this->joinCandidateCursor->init();
}

bool OuterJoinExecutor::hasNext() {
	while(!this->joinCandidateCursor->finished()){
		if(!hasNextLeftRecord()){
			this->joinCandidateCursor->inc();

			delete this->leftRecord, this->leftRecord = nullptr;
			this->joined = false;
			continue;
		}

		// right scan for left record
		if(!this->right->hasNext()){
			delete this->nextRecord, this->nextRecord = nullptr;

			if(!this->joined){
				const ScanResultMetadata* rightMeta = this->right->getMetadata();
				const ArrayList<ScanResultFieldMetadata>* l = rightMeta->getList();

				// TODO add null
				int size = l->size();
				delete this->nextRecord, this->nextRecord = nullptr;
				this->nextRecord = this->leftRecord->joinEmptyRecord(size);
				this->joined = false;

				resetLeftRecord();

				return true;
			}

			this->joined = false;
			resetLeftRecord();

			continue;
		}

		const CdbRecord* rightRecord = this->right->next();

		delete this->nextRecord, this->nextRecord = nullptr;
		this->nextRecord = this->leftRecord->joinRecord(rightRecord);
		this->joined = true;

		return true;
	}

	// TODO scan logic

	return false;
}

void OuterJoinExecutor::resetLeftRecord() {
	delete this->leftRecord;
	this->leftRecord = nullptr;
}

bool OuterJoinExecutor::hasNextLeftRecord() {
	if(this->leftRecord == nullptr && this->left->hasNext()){
		delete this->leftRecord, this->leftRecord = nullptr;

		const CdbRecord* rec = this->left->next();

		AbstractCdbValue* v = rec->copy();

		this->leftRecord = dynamic_cast<CdbRecord*>(rec->copy());
		onChangeLeft();
	}

	return this->leftRecord != nullptr;
}

void OuterJoinExecutor::onChangeLeft() {
	AbstractCdbKey* key = this->joinCandidateCursor->makeKey(this->leftRecord); __STP(key);

	this->right->reset(key);
	this->joined = false;
}

const CdbRecord* OuterJoinExecutor::next() {
	return this->nextRecord;
}

void OuterJoinExecutor::shutdown() {
	AbstractJoinExecutor::shutdown();
}

} /* namespace codablecash */
