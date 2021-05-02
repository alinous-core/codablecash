/*
 * ScanResultExecutor.cpp
 *
 *  Created on: 2021/01/21
 *      Author: iizuka
 */

#include "trx/scan/transaction_scan_result/ScanResultExecutor.h"

#include "trx/scan/transaction_scanner_join/IJoinLeftSource.h"

#include "trx/transaction_cache/OidKeyRecordCache.h"

#include "engine/CodableDatabase.h"
#include "engine/CdbLocalCacheManager.h"

#include "schema_table/record/table_record_local/LocalOidFactory.h"

#include "schema_table/record/table_record_value/CdbValueCaster.h"

#include "vm/VirtualMachine.h"

#include "vm/stack/VmStack.h"

#include "scan_select/scan_planner/base/ConditionsHolder.h"
#include "scan_select/scan_planner/base/SelectScanPlanner.h"

#include "scan_select/scan_condition/base/RootScanCondition.h"

#include "base/StackRelease.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"

#include "schema_table/record/table_record_value/CdbByteValue.h"

#include "instance/instance_gc/GcManager.h"

#include "engine/sc_analyze/AnalyzedType.h"

#include "instance/instance_ref/AbstractReference.h"

#include "instance/instance_ref/RefereceFactory.h"

#include "instance/instance_dom/DomArrayVariable.h"
namespace codablecash {

ScanResultExecutor::ScanResultExecutor(IJoinLeftSource* source, CodableDatabase* db) {
	this->source = source;
	this->db = db;
	this->cache = nullptr;
}

ScanResultExecutor::~ScanResultExecutor() {
	this->source->shutdown();

	delete this->source;
	this->db = nullptr;

	delete this->cache;
	this->cache = nullptr;
}

void ScanResultExecutor::execScan(VirtualMachine* vm) {
	if(this->source != nullptr){
		doExecScan(vm);
	}
}

void ScanResultExecutor::init(VirtualMachine* vm) {
	CdbLocalCacheManager* localCacheManager = this->db->getLocalCacheManager();
	this->cache = localCacheManager->createOidKeyRecordCache();

}

void ScanResultExecutor::doExecScan(VirtualMachine* vm) {
	assert(this->cache != nullptr);

	RootScanCondition* root = nullptr;

	SelectScanPlanner* planner = vm->getSelectPlanner();
	ConditionsHolder* cholder = planner->getConditions();
	root = cholder->getRoot();

	const ScanResultMetadata* metadata = this->source->getMetadata();

	this->source->start();
	while(this->source->hasNext()){
		const CdbRecord* record = this->source->next();

		if(!checkRecord(vm, root, record, metadata)){
			continue;
		}

		this->cache->insert(record);
	}
}

bool ScanResultExecutor::checkRecord(VirtualMachine* vm, RootScanCondition* root,
		const CdbRecord* record, const ScanResultMetadata* metadata) {
	AbstractCdbValue* v = root->evaluate(vm, record, metadata); __STP(v);

	AbstractCdbValue* bl = CdbValueCaster::cast(v, AbstractCdbValue::TYPE_BYTE); __STP(bl);
	CdbByteValue* byteV = dynamic_cast<CdbByteValue*>(bl);

	uint8_t i = byteV->getValue();
	return i > 0;
}

void ScanResultExecutor::putResult(VirtualMachine* vm) {
	GcManager* gc = vm->getGc();

	VmStack* stack = vm->topStack();

	AnalyzedType atype(AnalyzedType::TYPE_DOM_ARRAY);
	AbstractReference* ref = RefereceFactory::createReferenceFromAnalyzedType(stack, &atype, vm);
	stack->addInnerReference(ref);

	DomArrayVariable* variable = getRecordsVariable(vm);
	ref->substitute(variable, vm);
	// TODO:put record
}

DomArrayVariable* ScanResultExecutor::getRecordsVariable(VirtualMachine* vm) {
	DomArrayVariable* array = new(vm) DomArrayVariable(vm);

	return array;
}

} /* namespace codablecash */
