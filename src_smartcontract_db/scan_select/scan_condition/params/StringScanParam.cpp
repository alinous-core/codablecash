/*
 * StringScanParam.cpp
 *
 *  Created on: 2020/07/21
 *      Author: iizuka
 */

#include "scan_select/scan_condition/params/StringScanParam.h"

#include "base/UnicodeString.h"

#include "schema_table/record/table_record_value/CdbStringValue.h"

namespace codablecash {

StringScanParam::StringScanParam(const StringScanParam& inst) {
	this->value = new UnicodeString(inst.value);
	this->str = nullptr;
}

StringScanParam::StringScanParam(const UnicodeString* str) {
	this->value = new UnicodeString(str);
	this->str = nullptr;
}

StringScanParam::StringScanParam(const wchar_t* str) {
	this->value = new UnicodeString(str);
	this->str = nullptr;
}

StringScanParam::~StringScanParam() {
	delete this->value;
	delete this->str;
}

const UnicodeString* StringScanParam::toStringCode() noexcept {
	if(this->str == nullptr){
		delete this->str;
		this->str = new UnicodeString(L"'");
		this->str->append(this->value);
		this->str->append(L"'");
	}

	return this->str;
}

IValueProvider* StringScanParam::clone() const noexcept {
	return new StringScanParam(*this);
}

AbstractCdbValue* StringScanParam::evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata) {
	CdbStringValue* value = new CdbStringValue(this->value);

	return value;
}

} /* namespace codablecash */
