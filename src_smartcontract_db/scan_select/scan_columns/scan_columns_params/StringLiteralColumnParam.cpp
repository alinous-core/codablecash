/*
 * StringLiteralColumnParam.cpp
 *
 *  Created on: 2020/08/20
 *      Author: iizuka
 */

#include "scan_select/scan_columns/scan_columns_params/StringLiteralColumnParam.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "scan_select/scan_condition/params/StringScanParam.h"

#include "schema_table/record/table_record/CdbRecord.h"

namespace codablecash {

StringLiteralColumnParam::StringLiteralColumnParam(const UnicodeString* str) {
	this->value = new UnicodeString(str);
}

StringLiteralColumnParam::~StringLiteralColumnParam() {
	delete this->value;
}

const UnicodeString* StringLiteralColumnParam::toStringCode() noexcept {
	if(this->str == nullptr){
		this->str = new UnicodeString(L"'");
		this->str->append(this->value);
		this->str->append(L"'");
	}

	return this->str;
}

void StringLiteralColumnParam::scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord) {
	StringScanParam* cond = dynamic_cast<StringScanParam*>(toValueProvider()); __STP(cond);

	AbstractCdbValue* v = cond->evaluate(vm, record, metadata);
	newRecord->addValue(v);
}

IValueProvider* StringLiteralColumnParam::toValueProvider() {
	return new StringScanParam(this->value);
}

} /* namespace codablecash */
