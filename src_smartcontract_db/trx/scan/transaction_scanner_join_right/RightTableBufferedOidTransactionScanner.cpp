/*
 * RightTableBufferedOidTransactionScanner.cpp
 *
 *  Created on: 2020/12/24
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner_join_right/RightTableBufferedOidTransactionScanner.h"

#include "trx/transaction/CdbTransactionManager.h"
#include "trx/transaction/CdbTransaction.h"

#include "engine/CodableDatabase.h"
#include "engine/CdbLocalCacheManager.h"

#include "trx/transaction_cache/OidKeyRecordCache.h"

#include "trx/scan/transaction_scanner/TableTransactionScanner.h"

#include "base/StackRelease.h"

#include "btree/IBtreeScanner.h"

#include "schema_table/record/table_record/CdbRecord.h"
namespace codablecash {

RightTableBufferedOidTransactionScanner::RightTableBufferedOidTransactionScanner(ScanResultMetadata* metadata, CdbTransaction* trx,
		const CdbTable* table, const AbstractScanCondition* filterCondition)
		: AbstractRightTransactionScanner(metadata, trx, table, filterCondition){
	this->oidRecordCache = nullptr;
	this->scanner = nullptr;
	this->nextRecord = nullptr;
}

RightTableBufferedOidTransactionScanner::~RightTableBufferedOidTransactionScanner() {
	shutdown();
}

void RightTableBufferedOidTransactionScanner::start(VirtualMachine* vm) {
	CdbTransactionManager* trxManager = this->trx->getTrxManager();
	CodableDatabase* db = trxManager->getDb();

	CdbLocalCacheManager* cacheManager = db->getLocalCacheManager();

	this->oidRecordCache = cacheManager->createOidKeyRecordCache();

	TableTransactionScanner* scanner = this->trx->getTableTransactionScanner(this->table, this->metadata, this->filterCondition); __STP(scanner);
	scanner->start(vm);

	while(scanner->hasNext(vm)){
		const CdbRecord* rec = scanner->next(vm);

		this->oidRecordCache->insert(rec);
	}
}

void RightTableBufferedOidTransactionScanner::reset(const AbstractCdbKey* key) {
	delete this->scanner;
	this->scanner = this->oidRecordCache->getScanner();
}

bool RightTableBufferedOidTransactionScanner::hasNext(VirtualMachine* vm) {
	if(!this->scanner->hasNext()){
		return false;
	}

	const IBlockObject* obj = this->scanner->next();
	const CdbRecord* rec = dynamic_cast<const CdbRecord*>(obj);

	delete this->nextRecord;
	this->nextRecord = dynamic_cast<CdbRecord*>(rec->copy());

	return true;
}

const CdbRecord* RightTableBufferedOidTransactionScanner::next(VirtualMachine* vm) {
	this->nextRecord->initScanIdentity();
	return this->nextRecord;
}

void RightTableBufferedOidTransactionScanner::shutdown() {
	delete this->oidRecordCache;
	this->oidRecordCache = nullptr;

	delete this->scanner;
	this->scanner = nullptr;

	delete this->nextRecord;
	this->nextRecord = nullptr;
}

} /* namespace codablecash */
