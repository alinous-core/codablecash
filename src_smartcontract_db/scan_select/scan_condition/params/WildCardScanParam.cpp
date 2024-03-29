/*
 * WildCardScanParam.cpp
 *
 *  Created on: 2020/08/23
 *      Author: iizuka
 */

#include "scan_select/scan_condition/params/WildCardScanParam.h"

#include "base/UnicodeString.h"

namespace codablecash {

const UnicodeString WildCardScanParam::WILDCARD(L"*");

WildCardScanParam::WildCardScanParam() {

}

WildCardScanParam::~WildCardScanParam() {

}

const UnicodeString* WildCardScanParam::toStringCode() noexcept {
	return &WILDCARD;
}

IValueProvider* WildCardScanParam::clone() const noexcept {
	return new WildCardScanParam();
}

AbstractCdbValue* WildCardScanParam::evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata) {
	return nullptr;
}

} /* namespace codablecash */
