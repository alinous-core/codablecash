/*
 * InfinityScanParam.cpp
 *
 *  Created on: 2022/01/01
 *      Author: iizuka
 */

#include "scan_select/scan_condition/params/InfinityScanParam.h"

#include "schema_table/record/table_record_value/CdbInfinityValue.h"

namespace codablecash {

const UnicodeString InfinityScanParam::INFINITY_STR(L"infinity");

InfinityScanParam::InfinityScanParam(const InfinityScanParam& inst) {

}

InfinityScanParam::InfinityScanParam() {

}

InfinityScanParam::~InfinityScanParam() {

}

const UnicodeString* InfinityScanParam::toStringCode() noexcept {
	return &INFINITY_STR;
}

IValueProvider* InfinityScanParam::clone() const noexcept {
	return new InfinityScanParam(*this);
}

AbstractCdbValue* InfinityScanParam::evaluate(VirtualMachine* vm,
		const CdbRecord* record, const ScanResultMetadata* metadata) {
	return new CdbInfinityValue();
}

} /* namespace codablecash */
