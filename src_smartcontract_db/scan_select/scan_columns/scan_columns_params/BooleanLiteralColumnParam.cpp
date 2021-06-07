/*
 * BooleanLiteralColumnParam.cpp
 *
 *  Created on: 2020/08/21
 *      Author: iizuka
 */

#include "scan_select/scan_columns/scan_columns_params/BooleanLiteralColumnParam.h"

#include "base/UnicodeString.h"

namespace codablecash {

const UnicodeString BooleanLiteralColumnParam::TRUE(L"true");
const UnicodeString BooleanLiteralColumnParam::FALSE(L"false");

BooleanLiteralColumnParam::BooleanLiteralColumnParam(bool value) {
	this->value = value;
}

BooleanLiteralColumnParam::~BooleanLiteralColumnParam() {

}

const UnicodeString* BooleanLiteralColumnParam::toStringCode() noexcept {
	return this->value ? &TRUE: &FALSE;;
}

void BooleanLiteralColumnParam::scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord) {
	// FIXME
}


} /* namespace codablecash */
