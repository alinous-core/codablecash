/*
 * AbstractTransactionScanner.cpp
 *
 *  Created on: 2020/06/10
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner/AbstractTransactionScanner.h"

#include "trx/transaction/CdbTransaction.h"

#include "trx/scan/transaction_update_cache/InsertRecordsCacheCursor.h"
#include "trx/scan/transaction_update_cache/TransactionUpdateCache.h"

#include "trx/scan/transaction_scan_result/ScanResultMetadata.h"

#include "schema_table/table/CdbTable.h"

#include "scan_select/scan_condition/base/AbstractScanCondition.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "trx/scan/transaction_update_cache/UpdatedRecordCursor.h"

namespace codablecash {

AbstractTransactionScanner::AbstractTransactionScanner(const ScanResultMetadata* metadata,
		CdbTransaction* trx, const CdbTable* table, const AbstractScanCondition* filterCondition) {
	this->metadata = new ScanResultMetadata(*metadata);
	this->trx = trx;
	this->table = table;
	this->filterCondition = filterCondition != nullptr ? filterCondition->cloneCondition() : nullptr;

	this->scanedInserted = false;;
	this->scanedUpdated = false;;

	this->insertedCacheCursor = nullptr;
	this->updatedCacheCurser = nullptr;

	this->insertedCacheCursor = nullptr;
	if(table != nullptr){
		this->trxCache = this->trx->getUpdateCache();

		this->insertedCacheCursor = this->trxCache->newInsertedRecordsCursor(table);
		this->updatedCacheCurser = this->trxCache->getUpdatedRecordCursor(table);
	}
}

AbstractTransactionScanner::~AbstractTransactionScanner() {
	delete metadata;
	this->trx = nullptr;
	this->table = nullptr;
	delete this->insertedCacheCursor;
	delete this->updatedCacheCurser;
	delete this->filterCondition;
}

bool AbstractTransactionScanner::hasInsertedRecord() const noexcept {
	if(this->scanedInserted){
		return false;
	}

	bool result = this->insertedCacheCursor->hasNext();
	if(!result){
		this->scanedInserted = true;
	}

	return result;
}

void AbstractTransactionScanner::restartScan(VirtualMachine* vm) {
	if(table != nullptr){
		delete this->insertedCacheCursor, this->insertedCacheCursor = nullptr;
		delete this->updatedCacheCurser, this->updatedCacheCurser = nullptr;

		this->scanedInserted = false;
		this->scanedUpdated = false;

		this->insertedCacheCursor = this->trxCache->newInsertedRecordsCursor(table);
		this->updatedCacheCurser = this->trxCache->getUpdatedRecordCursor(table);
	}
}

const CdbRecord* AbstractTransactionScanner::nextInsertedRecord() noexcept {
	return this->insertedCacheCursor->next();
}

bool AbstractTransactionScanner::hasUpdatedRecord() const noexcept {
	/*if(this->scanedUpdated){
		return false;
	}
	bool result = this->updatedCacheCurser->hasNext();
	if(!result){
		this->scanedUpdated = true;
	}

	return result;*/
	return this->updatedCacheCurser->hasNext();
}

const CdbRecord* AbstractTransactionScanner::nextUpdatedRecord() noexcept {
	return this->updatedCacheCurser->next();
}

bool AbstractTransactionScanner::checkUpdated(const CdbRecord* record) const noexcept {
	const CdbOid* oid = record->getOid();

	return this->trxCache->isDeleted(this->table, oid) || this->trxCache->isUpdated(this->table, oid);
}

} /* namespace codablecash */
