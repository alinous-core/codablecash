/*
 * NumberLiteralColumnParam.cpp
 *
 *  Created on: 2020/08/20
 *      Author: iizuka
 */

#include "scan_select/scan_columns/scan_columns_params/NumberLiteralColumnParam.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"
#include "base/Long.h"

#include "scan_select/scan_condition/params/NumericScanParam.h"

#include "schema_table/record/table_record/CdbRecord.h"


namespace codablecash {

NumberLiteralColumnParam::NumberLiteralColumnParam(int64_t longv) {
	this->longv = longv;
}

NumberLiteralColumnParam::~NumberLiteralColumnParam() {

}

const UnicodeString* NumberLiteralColumnParam::toStringCode() noexcept {
	if(this->str == nullptr){
		this->str = Long::toString(this->longv, 10);
	}

	return this->str;
}

void NumberLiteralColumnParam::scanColumns(VirtualMachine* vm, const CdbRecord* record,	const ScanResultMetadata* metadata, CdbRecord* newRecord) {
	NumericScanParam* cond = dynamic_cast<NumericScanParam*>(toValueProvider()); __STP(cond);

	AbstractCdbValue* v = cond->evaluate(vm, record, metadata);
	newRecord->addValue(v);
}

IValueProvider* NumberLiteralColumnParam::toValueProvider() {
	return new NumericScanParam(this->longv);
}

} /* namespace codablecash */
