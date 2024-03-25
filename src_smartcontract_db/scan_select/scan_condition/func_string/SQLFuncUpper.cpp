/*
 * SQLFuncUpper.cpp
 *
 *  Created on: 2023/06/05
 *      Author: iizuka
 */

#include "scan_select/scan_condition/func_string/SQLFuncUpper.h"
#include "scan_select/scan_condition/base/IValueProvider.h"

#include "base/UnicodeString.h"
#include "base/ArrayList.h"
#include "base/StackRelease.h"

#include "lang_sql/sql_expression/SQLFunctionCall.h"

#include "lang/sc_expression/VariableIdentifier.h"

#include "engine/sc_analyze/AnalyzeContext.h"
#include "engine/sc_analyze/ValidationError.h"

#include "schema_table/record/table_record_value/CdbValueCaster.h"
#include "schema_table/record/table_record_value/AbstractCdbValue.h"

#include "schema_table/record/table_record_value/CdbStringValue.h"
namespace codablecash {

SQLFuncUpper::SQLFuncUpper() : AbstractSQLFunctionBody(new UnicodeString(UPPER)) {

}

SQLFuncUpper::~SQLFuncUpper() {

}

void SQLFuncUpper::analyze(AnalyzeContext *actx, SQLFunctionCall *fcall) {
	const ArrayList<AbstractSQLExpression>* list = fcall->getArguments();

	int size = list->size();
	if(size != 1){
		const UnicodeString* fname = fcall->getName()->getName();
		actx->addValidationError(ValidationError::SQL_CONDITION_WRONG_FUNC_ARG_NUM, fcall, L"Number of SQL function {0}'s arguments is wrong. ", {fname});
		return;
	}
}

AbstractCdbValue* SQLFuncUpper::evaluate(VirtualMachine *vm, const CdbRecord *record, const ArrayList<IValueProvider> *list, const ScanResultMetadata *metadata) {
	IValueProvider* provider = list->get(0);

	AbstractCdbValue* value = provider->evaluate(vm, record, metadata); __STP(value);

	if(value == nullptr || value->isNull()){
		return nullptr;
	}

	AbstractCdbValue* cdbstrv = CdbValueCaster::cast(value, AbstractCdbValue::TYPE_STRING); __STP(cdbstrv);
	CdbStringValue* strv = dynamic_cast<CdbStringValue*>(cdbstrv);

	const UnicodeString* str = strv->getValue();
	UnicodeString* upperStr = str->toUpperCase(); __STP(upperStr);

	return new CdbStringValue(upperStr);
}

} /* namespace codablecash */
