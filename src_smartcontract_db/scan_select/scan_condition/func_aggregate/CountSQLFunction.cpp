/*
 * CountSQLFunction.cpp
 *
 *  Created on: 2021/10/31
 *      Author: iizuka
 */

#include "scan_select/scan_condition/func_aggregate/CountSQLFunction.h"
#include "scan_select/scan_condition/func_aggregate/CountSQLFunctionCache.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "schema_table/record/table_record/CdbGroupedRecord.h"

#include "trx/scan/transaction_scan_result/ScanResultMetadata.h"
#include "trx/scan/transaction_scan_result/GroupedScanResultMetadata.h"

#include "trx/transaction_cache_array/OidArrayCache.h"
#include "trx/transaction_cache_array/OidArrayCacheScanner.h"

#include "trx/transaction_cache/OidKeyRecordCache.h"

#include "scan_select/scan_condition/base/IValueProvider.h"

#include "scan_select/scan_condition/params/ColumnIdentifierScanParam.h"

#include "schema_table/record/table_record_key/AbstractCdbKey.h"
#include "schema_table/record/table_record_key/CdbOidKey.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"
#include "schema_table/record/table_record_value/CdbLongValue.h"

#include "vm/VirtualMachine.h"

#include "engine/CdbLocalCacheManager.h"
#include "engine/CodableDatabase.h"

#include "lang_sql/sql_expression/SQLFunctionCall.h"
#include "lang/sc_expression/VariableIdentifier.h"

#include "engine/sc_analyze/ValidationError.h"
#include "engine/sc_analyze/AnalyzeContext.h"

namespace codablecash {

CountSQLFunction::CountSQLFunction() : AbstractSQLAggregateFunctionBody(new UnicodeString(L"count")){

}

CountSQLFunction::~CountSQLFunction() {

}

void CountSQLFunction::analyze(AnalyzeContext* actx, SQLFunctionCall* fcall) {
	const ArrayList<AbstractSQLExpression>* list = fcall->getArguments();

	int size = list->size();
	if(size != 1){
		const UnicodeString* fname = fcall->getName()->getName();
		actx->addValidationError(ValidationError::SQL_CONDITION_WRONG_FUNC_ARG_NUM, fcall, L"Number of SQL function {0}'s arguments is wrong. ", {fname});
		return;
	}
}

AbstractCdbValue* CountSQLFunction::doGroupEvaluate(VirtualMachine* vm,
		const CdbGroupedRecord* record, const ArrayList<IValueProvider>* list, const GroupedScanResultMetadata* metadata) {

	IValueProvider* provider = list->get(0);

	if(provider->isColumn()){
		return countNotNull(vm, record, list, metadata);
	}

	return countAll(vm, record, list, metadata);
}

AbstractCdbValue* CountSQLFunction::countNotNull(VirtualMachine* vm,
		const CdbGroupedRecord* record, const ArrayList<IValueProvider>* list,
		const GroupedScanResultMetadata* metadata) {
	IValueProvider* provider = list->get(0);

	ColumnIdentifierScanParam* column = dynamic_cast<ColumnIdentifierScanParam*>(provider);
	if(column->isDistinct()){
		return countDistinct(vm, record, list, metadata);
	}

	ScanResultMetadata* baseMetadata = metadata->getBase();

	OidArrayCache* oidCache = record->getOidCache();
	OidKeyRecordCache* recordCache = record->getOidKeyRecordCache();

	uint64_t index = record->getIndexOfCache();

	int64_t result = 0;
	OidArrayCacheScanner* scanner = oidCache->getScanner(index); __STP(scanner);
	while(scanner->hasNext()){
		const CdbOid* oid = scanner->next();
		CdbOidKey key(oid);

		const CdbRecord* rec = recordCache->findRecord(&key);

		AbstractCdbValue* value = column->evaluate(vm, rec, baseMetadata); __STP(value);
		if(!value->isNull()){
			result++;
		}
	}

	return new CdbLongValue(result);
}

AbstractCdbValue* CountSQLFunction::countDistinct(VirtualMachine* vm,
		const CdbGroupedRecord* record, const ArrayList<IValueProvider>* list,
		const GroupedScanResultMetadata* metadata) {
	CodableDatabase* db = vm->getDb();
	CdbLocalCacheManager* localCacheManager = db->getLocalCacheManager();

	CountSQLFunctionCache* countCache = localCacheManager->createCountSQLFunctionCache(); __STP(countCache);

	ScanResultMetadata* baseMetadata = metadata->getBase();

	IValueProvider* provider = list->get(0);

	ColumnIdentifierScanParam* column = dynamic_cast<ColumnIdentifierScanParam*>(provider);

	OidArrayCache* oidCache = record->getOidCache();
	OidKeyRecordCache* recordCache = record->getOidKeyRecordCache();

	uint64_t index = record->getIndexOfCache();

	int64_t result = 0;
	OidArrayCacheScanner* scanner = oidCache->getScanner(index); __STP(scanner);
	while(scanner->hasNext()){
		const CdbOid* oid = scanner->next();

		CdbOidKey key(oid);

		const CdbRecord* rec = recordCache->findRecord(&key);

		AbstractCdbValue* value = column->evaluate(vm, rec, baseMetadata); __STP(value);
		AbstractCdbKey* vkey = value->toKey(); __STP(vkey);

		if(!countCache->hasValue(vkey)){
			result++;
			countCache->addValue(vkey);
		}
	}

	return new CdbLongValue(result);
}

AbstractCdbValue* CountSQLFunction::countAll(VirtualMachine* vm,
		const CdbGroupedRecord* record, const ArrayList<IValueProvider>* list,
		const GroupedScanResultMetadata* metadata) {
	ScanResultMetadata* baseMetadata = metadata->getBase();

	OidArrayCache* oidCache = record->getOidCache();
	OidKeyRecordCache* recordCache = record->getOidKeyRecordCache();

	uint64_t index = record->getIndexOfCache();

	int64_t result = 0;
	OidArrayCacheScanner* scanner = oidCache->getScanner(index); __STP(scanner);
	while(scanner->hasNext()){
		const CdbOid* oid = scanner->next();

		result++;
	}

	return new CdbLongValue(result);
}

} /* namespace codablecash */
