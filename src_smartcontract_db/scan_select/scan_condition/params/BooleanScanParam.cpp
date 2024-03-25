/*
 * BooleanScanParam.cpp
 *
 *  Created on: 2020/07/27
 *      Author: iizuka
 */

#include "scan_select/scan_condition/params/BooleanScanParam.h"

#include "base/UnicodeString.h"

#include "schema_table/record/table_record_value/CdbBooleanValue.h"

namespace codablecash {

const UnicodeString BooleanScanParam::TRUE(L"true");
const UnicodeString BooleanScanParam::FALSE(L"false");

BooleanScanParam::BooleanScanParam(const BooleanScanParam& inst) {
	this->value = inst.value;
}


BooleanScanParam::BooleanScanParam(bool value) {
	this->value = value;
}

BooleanScanParam::~BooleanScanParam() {

}

const UnicodeString* BooleanScanParam::toStringCode() noexcept {
	return this->value ? &TRUE: &FALSE;
}

IValueProvider* BooleanScanParam::clone() const noexcept {
	return new BooleanScanParam(*this);
}

AbstractCdbValue* BooleanScanParam::evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata) {
	return new CdbBooleanValue(this->value);
}

} /* namespace codablecash */
