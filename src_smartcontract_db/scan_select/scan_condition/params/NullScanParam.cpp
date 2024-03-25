/*
 * NullScanParam.cpp
 *
 *  Created on: 2021/01/13
 *      Author: iizuka
 */

#include "scan_select/scan_condition/params/NullScanParam.h"

#include "schema_table/record/table_record_value/CdbNullValue.h"

namespace codablecash {

const UnicodeString NullScanParam::NULL_STR(L"null");

NullScanParam::NullScanParam(const NullScanParam& inst) {

}

NullScanParam::NullScanParam() {
}

NullScanParam::~NullScanParam() {

}

const UnicodeString* NullScanParam::toStringCode() noexcept {
	return &NULL_STR;
}

IValueProvider* NullScanParam::clone() const noexcept {
	return new NullScanParam(*this);
}

AbstractCdbValue* NullScanParam::evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata) {
	return new CdbNullValue();
}

} /* namespace codablecash */
