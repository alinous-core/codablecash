/*
 * ScanResultExecutor.cpp
 *
 *  Created on: 2021/01/21
 *      Author: iizuka
 */

#include "trx/scan/transaction_scan_result/ScanResultExecutor.h"

#include "trx/scan/transaction_scanner_join/IJoinLeftSource.h"

#include "engine/CodableDatabase.h"
#include "engine/CdbLocalCacheManager.h"

#include "schema_table/record/table_record_local/LocalOidFactory.h"

#include "vm/VirtualMachine.h"

namespace codablecash {

ScanResultExecutor::ScanResultExecutor(IJoinLeftSource* source, CodableDatabase* db) {
	this->source = source;
	this->db = db;
}

ScanResultExecutor::~ScanResultExecutor() {
	this->source->shutdown();

	delete this->source;
	this->db = nullptr;
}

void ScanResultExecutor::execScan(VirtualMachine* vm) {
	if(this->source != nullptr){
		doExecScan(vm);
	}
}

void ScanResultExecutor::init(VirtualMachine* vm) {

}

void ScanResultExecutor::doExecScan(VirtualMachine* vm) {
	LocalOidFactory* localOidFactory = this->db->getLocalOidFactory();
	CdbLocalCacheManager* cacheManager = this->db->getLocalCacheManager();

	this->source->start();
	while(this->source->hasNext()){
		const CdbRecord* record = this->source->next();


	}
}

} /* namespace codablecash */
