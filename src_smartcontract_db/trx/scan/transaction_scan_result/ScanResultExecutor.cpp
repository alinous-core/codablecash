/*
 * ScanResultExecutor.cpp
 *
 *  Created on: 2021/01/21
 *      Author: iizuka
 */

#include "trx/scan/transaction_scan_result/ScanResultExecutor.h"

#include "trx/scan/transaction_scanner_join/IJoinLeftSource.h"

#include "trx/transaction_cache/OidKeyRecordCache.h"
#include "trx/transaction_cache/GroupCache.h"
#include "trx/transaction_cache/GroupCacheScanner.h"

#include "engine/CodableDatabase.h"
#include "engine/CdbLocalCacheManager.h"

#include "schema_table/record/table_record_local/LocalOidFactory.h"

#include "schema_table/record/table_record_value/CdbValueCaster.h"
#include "schema_table/record/table_record_value/AbstractCdbValue.h"
#include "schema_table/record/table_record_value/CdbByteValue.h"

#include "vm/VirtualMachine.h"

#include "vm/stack/VmStack.h"

#include "scan_select/scan_planner/base/ConditionsHolder.h"
#include "scan_select/scan_planner/base/SelectScanPlanner.h"

#include "scan_select/scan_condition/base/RootScanCondition.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "instance/instance_gc/GcManager.h"

#include "engine/sc_analyze/AnalyzedType.h"

#include "instance/instance_ref/AbstractReference.h"
#include "instance/instance_ref/RefereceFactory.h"

#include "instance/instance_dom/DomArrayVariable.h"
#include "instance/instance_dom/DomVariableInstance.h"

#include "btree/IBtreeScanner.h"

#include "filestore_block/IBlockObject.h"

#include "schema_table/record/table_record/CdbRecord.h"
#include "schema_table/record/table_record/CdbGroupedRecord.h"

#include "trx/scan/transaction_scan_result/ScanResultMetadata.h"
#include "trx/scan/transaction_scan_result/ScanResultFieldMetadata.h"

#include "scan_select/scan_planner/base/GroupByPlanner.h"


namespace codablecash {

ScanResultExecutor::ScanResultExecutor(IJoinLeftSource* source, CodableDatabase* db) {
	this->source = source;
	this->db = db;
	this->cache = nullptr;
	this->groupKeyCache = nullptr;
}

ScanResultExecutor::~ScanResultExecutor() {
	this->source->shutdown();

	delete this->source;
	this->db = nullptr;

	delete this->cache;
	this->cache = nullptr;

	delete this->groupKeyCache;
	this->groupKeyCache = nullptr;
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

	// scan
	this->source->start();
	while(this->source->hasNext()){
		const CdbRecord* record = this->source->next();

		if(checkRecord(vm, root, record, metadata)){
			this->cache->insert(record);
		}
	}

	// check group by
	if(planner->getGroupPlan() != nullptr){
		doGroupBy(vm, planner);
	}
}

void ScanResultExecutor::doGroupBy(VirtualMachine* vm, SelectScanPlanner* planner) {
	GroupByPlanner* groupByPlan = planner->getGroupPlan();

	CdbLocalCacheManager* localCacheManager = this->db->getLocalCacheManager();

	this->groupKeyCache = new GroupCache(localCacheManager);
	this->groupKeyCache->init(vm);


	const ScanResultMetadata* metadata = this->source->getMetadata();

	IBtreeScanner* scanner = this->cache->getScanner(); __STP(scanner);
	scanner->begin();

	while(scanner->hasNext()){
		const IBlockObject* obj = scanner->next();
		const CdbRecord* record = dynamic_cast<const CdbRecord*>(obj);

		this->groupKeyCache->groupRecord(vm, record, metadata, groupByPlan);
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

void ScanResultExecutor::putResult(VirtualMachine* vm, SelectScanPlanner* planner) {
	// check group by
	if(planner->getGroupPlan() != nullptr){
		putResultGroupBy(vm, planner);
		return;
	}

	GcManager* gc = vm->getGc();
	VmStack* stack = vm->topStack();

	AnalyzedType atype(AnalyzedType::TYPE_DOM_ARRAY);
	AbstractReference* ref = RefereceFactory::createReferenceFromAnalyzedType(stack, &atype, vm);
	stack->addInnerReference(ref);

	DomArrayVariable* variable = getRecordsVariable(vm);
	ref->substitute(variable, vm);
}

DomArrayVariable* ScanResultExecutor::getRecordsVariable(VirtualMachine* vm) {
	DomArrayVariable* array = new(vm) DomArrayVariable(vm);

	const ScanResultMetadata* metadata = this->source->getMetadata();
	const ArrayList<ScanResultFieldMetadata>* fldMetadataList = metadata->getList();
	ArrayList<const UnicodeString> nameList;

	// FIXME not name list
	int maxLoop = fldMetadataList->size();
	for(int i = 0; i != maxLoop; ++i){
		ScanResultFieldMetadata* fldMeta = fldMetadataList->get(i);
		const UnicodeString* name = fldMeta->getDisplayName();

		nameList.addElement(name);
	}

	IBtreeScanner* scanner = this->cache->getScanner(); __STP(scanner);
	scanner->begin();

	while(scanner->hasNext()){
		const IBlockObject* obj = scanner->next();
		const CdbRecord* record = dynamic_cast<const CdbRecord*>(obj);

		// FIXME scan by column holder

		DomVariableInstance* dom = record->toDomInstance(vm, &nameList);
		array->add(vm, dom);
	}

	return array;
}

void ScanResultExecutor::putResultGroupBy(VirtualMachine* vm, SelectScanPlanner* planner) {
	CdbLocalCacheManager* localCacheManager = this->db->getLocalCacheManager();
	OidKeyRecordCache* newResultCache = localCacheManager->createOidKeyRecordCache(); __STP(newResultCache);

	const ScanResultMetadata* metadata = this->source->getMetadata(); // base record metadata


	GroupByPlanner* gplan = planner->getGroupPlan();
	ScanColumnHolder* scanColumns = planner->getColumnHolder();

	GroupCacheScanner* scanner = this->groupKeyCache->getScanner(); __STP(scanner);

	while(scanner->hasNext()){
		const CdbGroupedRecord* grecord = scanner->next(this->cache);

		CdbRecord* record = scanResultColumns(vm, scanColumns, grecord, nullptr);
	}

	delete this->cache, this->cache = nullptr;

}

CdbRecord* ScanResultExecutor::scanResultColumns(VirtualMachine* vm, ScanColumnHolder* scanColumns, const CdbRecord* record,
		const ScanResultMetadata* metadata) {
}

} /* namespace codablecash */
