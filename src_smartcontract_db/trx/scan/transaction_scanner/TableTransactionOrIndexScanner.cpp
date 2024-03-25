/*
 * TableTransactionOrIndexScanner.cpp
 *
 *  Created on: 2020/12/11
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner/TableTransactionOrIndexScanner.h"

#include "schema_table/table_store/TableStore.h"

#include "engine/CodableDatabase.h"
#include "engine/CdbLocalCacheManager.h"

#include "vm/VirtualMachine.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/record/table_record/ScanRecordIndentity.h"

#include "trx/transaction_cache/RecordScanIdentityCache.h"
namespace codablecash {

TableTransactionOrIndexScanner::TableTransactionOrIndexScanner(ScanResultMetadata* metadata, CdbTransaction* trx,
		TableStore* tableStore, const AbstractScanCondition* filterCondition, const OrIndexAccessCollection* index)
		: AbstractLeftSourceTransactionScanner(metadata, trx, tableStore->getTable(), filterCondition){
	this->index = index;
	this->pos = 0;
	this->cache = nullptr;
	this->nextRecord = nullptr;
}

TableTransactionOrIndexScanner::~TableTransactionOrIndexScanner() {
	shutdown();
	this->index = nullptr;

	setRecord(nullptr);
}

void TableTransactionOrIndexScanner::start(VirtualMachine* vm) {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		IJoinLeftSource* source = this->list.get(i);

		source->start(vm);
	}

	CodableDatabase* db = vm->getDb();
	CdbLocalCacheManager* cacheManager = db->getLocalCacheManager();

	this->cache = cacheManager->createRecordScanIdentityCache();
}

void TableTransactionOrIndexScanner::restartScan(VirtualMachine* vm) {
	AbstractLeftSourceTransactionScanner::restartScan(vm);

	this->pos = 0;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		IJoinLeftSource* source = this->list.get(i);
		source->restartScan(vm);
	}

	CodableDatabase* db = vm->getDb();
	CdbLocalCacheManager* cacheManager = db->getLocalCacheManager();

	delete this->cache;
	this->cache = cacheManager->createRecordScanIdentityCache();
}

bool TableTransactionOrIndexScanner::hasNext(VirtualMachine* vm) {
	while(this->pos < this->list.size()){
		IJoinLeftSource* source = this->list.get(this->pos);
		if(!source->hasNext(vm)){
			this->pos++;
			continue;
		}

		const CdbRecord* record = source->next(vm);

		ScanRecordIndentity* scanId = record->getScanIdentity();
		bool alreadyScanned = this->cache->hasKey(scanId);
		if(alreadyScanned){
			continue;
		}
		this->cache->insert(scanId);

		setRecord(new CdbRecord(*record));

		return true;
	}

	return false;
}

const CdbRecord* TableTransactionOrIndexScanner::next(VirtualMachine* vm) {
	return this->nextRecord;
}

void TableTransactionOrIndexScanner::shutdown() {
	if(!this->list.isEmpty()){
		this->list.deleteElements();
		this->list.reset();

		delete this->cache, this->cache = nullptr;
	}
}

void TableTransactionOrIndexScanner::addLeftSource(IJoinLeftSource* source) noexcept {
	this->list.addElement(source);
}

void TableTransactionOrIndexScanner::setRecord(CdbRecord* rec) noexcept {
	delete this->nextRecord;
	this->nextRecord = rec;
}

} /* namespace codablecash */
