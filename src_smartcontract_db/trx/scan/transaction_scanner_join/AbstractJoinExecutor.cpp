/*
 * AbstractJoinExecutor.cpp
 *
 *  Created on: 2020/08/23
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner_join/AbstractJoinExecutor.h"

#include "trx/scan/transaction_scanner_join/IJoinLeftSource.h"
#include "trx/scan/transaction_scanner_join/IJoinRightSource.h"

#include "scan_select/scan_planner/scanner/ctx/ScanJoinContext.h"

#include "trx/scan/transaction_scan_result/ScanResultMetadata.h"

#include "scan_select/scan_condition/base/AbstractScanCondition.h"

#include "schema_table/record/table_record_local/LocalOidFactory.h"
#include "schema_table/record/table_record/CdbRecord.h"
#include "schema_table/record/table_record_local/LocalCdbOid.h"

#include "base/StackRelease.h"

namespace codablecash {

AbstractJoinExecutor::AbstractJoinExecutor(IJoinLeftSource* left, IJoinRightSource* right, ScanResultMetadata* metadata
		, ScanJoinContext* context, AbstractScanCondition* filterCondition, LocalOidFactory* localOidFactory) {
	this->left = left;
	this->right = right;
	this->metadata = new ScanResultMetadata(*metadata);
	this->context = context;
	this->filterCondition = filterCondition != nullptr ? filterCondition->cloneCondition() : nullptr;
	this->localOidFactory = localOidFactory;
}

AbstractJoinExecutor::~AbstractJoinExecutor() {
	delete this->left;
	delete this->right;
	delete this->metadata;
	delete this->context;
	delete this->filterCondition;
	this->localOidFactory = nullptr;
}

void AbstractJoinExecutor::start() {
	this->left->start();
	this->right->start();
}

void AbstractJoinExecutor::shutdown() {
	this->left->shutdown();
	this->right->shutdown();
}

void AbstractJoinExecutor::setLocalOid(CdbRecord* record) {
	LocalCdbOid* localOid = this->localOidFactory->createLocalOid(); __STP(localOid);

	record->setOid(localOid);
}

} /* namespace codablecash */
