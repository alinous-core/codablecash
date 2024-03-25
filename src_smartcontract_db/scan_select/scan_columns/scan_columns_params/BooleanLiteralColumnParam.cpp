/*
 * BooleanLiteralColumnParam.cpp
 *
 *  Created on: 2020/08/21
 *      Author: iizuka
 */

#include "scan_select/scan_columns/scan_columns_params/BooleanLiteralColumnParam.h"

#include "base/UnicodeString.h"

#include "scan_select/scan_condition/params/BooleanScanParam.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "base/StackRelease.h"

namespace codablecash {

const UnicodeString BooleanLiteralColumnParam::TRUE(L"true");
const UnicodeString BooleanLiteralColumnParam::FALSE(L"false");

BooleanLiteralColumnParam::BooleanLiteralColumnParam(bool value) {
	this->value = value;
	this->cond = nullptr;
}

BooleanLiteralColumnParam::~BooleanLiteralColumnParam() {
	delete this->cond;
}

const UnicodeString* BooleanLiteralColumnParam::toStringCode() noexcept {
	return this->value ? &TRUE: &FALSE;;
}

void BooleanLiteralColumnParam::scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord) {
	BooleanScanParam* param = dynamic_cast<BooleanScanParam*>(toValueProvider()); __STP(param);

	AbstractCdbValue* value = param->evaluate(vm, record, metadata);
	newRecord->addValue(value);
}

IValueProvider* BooleanLiteralColumnParam::toValueProvider() {
	return new BooleanScanParam(this->value);
}

} /* namespace codablecash */
