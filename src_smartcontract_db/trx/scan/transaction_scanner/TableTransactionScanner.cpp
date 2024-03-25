/*
 * TableTransactionScan.cpp
 *
 *  Created on: 2020/06/10
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner/TableTransactionScanner.h"

#include "trx/transaction/CdbTransaction.h"

#include "scan_select/scan/RecordScanner.h"

#include "schema_table/table_store/TableStore.h"

#include "engine_lock/AbstractLockHandle.h"
#include "engine_lock/AbstractDatabaseLock.h"

#include "schema_table/record/table_record/CdbRecord.h"
namespace codablecash {

TableTransactionScanner::TableTransactionScanner(const ScanResultMetadata* metadata, CdbTransaction* trx, TableStore* tableStore, const AbstractScanCondition* filterCondition)
	: AbstractLeftSourceTransactionScanner(metadata, trx, tableStore->getTable(), filterCondition) {
	this->tableStore = tableStore;
	this->internalScanner = nullptr;
	this->nextRecord = nullptr;
	this->scanedStore = false;
	this->lockHandle = nullptr;
}

TableTransactionScanner::~TableTransactionScanner() {
	shutdown();

	this->tableStore = nullptr;
	delete this->nextRecord, this->nextRecord = nullptr;
}

void TableTransactionScanner::shutdown() {
	if(this->internalScanner != nullptr){
		this->internalScanner->shutdown();
		delete this->internalScanner;
		this->internalScanner = nullptr;

		AbstractDatabaseLock* lock = this->lockHandle->getLock();
		lock->unclockHandle(this->lockHandle);
		this->lockHandle = nullptr;
	}
}

void TableTransactionScanner::start(VirtualMachine* vm) {
	this->lockHandle = this->tableStore->readLock();

	this->internalScanner = new RecordScanner(this->tableStore);
	this->internalScanner->start();

}

void TableTransactionScanner::restartScan(VirtualMachine* vm) {
	AbstractLeftSourceTransactionScanner::restartScan(vm);

	delete this->internalScanner;this->internalScanner = nullptr;
	this->scanedStore = false;

	this->internalScanner = new RecordScanner(this->tableStore);
	this->internalScanner->start();
}

bool TableTransactionScanner::hasNext(VirtualMachine* vm) {
	if(!this->scanedStore){
		while(this->internalScanner->hasNext()){
			// check updated

			delete this->nextRecord, this->nextRecord = nullptr;
			this->nextRecord = dynamic_cast<CdbRecord*>(this->internalScanner->next()->copy());

			if(checkUpdated(this->nextRecord)){
				continue;
			}

			return true;
		}
		this->scanedStore = true;
	}

	if(hasInsertedRecord()){
		delete this->nextRecord, this->nextRecord = nullptr;
		this->nextRecord = dynamic_cast<CdbRecord*>(nextInsertedRecord()->copy());
		return true;
	}

	if(hasUpdatedRecord()){
		delete this->nextRecord, this->nextRecord = nullptr;
		this->nextRecord = dynamic_cast<CdbRecord*>(nextUpdatedRecord()->copy());
		return true;
	}

	return false;
}

const CdbRecord* TableTransactionScanner::next(VirtualMachine* vm) {
	this->nextRecord->initScanIdentity();
	return this->nextRecord;
}


} /* namespace codablecash */
