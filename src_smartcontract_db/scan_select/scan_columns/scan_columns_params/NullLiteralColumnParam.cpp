/*
 * NullLiteralColumnParam.cpp
 *
 *  Created on: 2021/12/11
 *      Author: iizuka
 */

#include "scan_select/scan_columns/scan_columns_params/NullLiteralColumnParam.h"

#include "scan_select/scan_condition/params/NullScanParam.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "schema_table/record/table_record/CdbRecord.h"

namespace codablecash {

NullLiteralColumnParam::NullLiteralColumnParam() {
	this->cond = nullptr;
}

NullLiteralColumnParam::~NullLiteralColumnParam() {
	delete this->cond;
}

const UnicodeString* NullLiteralColumnParam::toStringCode() noexcept {
	NullScanParam param;
	return param.toStringCode();
}

void NullLiteralColumnParam::scanColumns(VirtualMachine* vm,
		const CdbRecord* record, const ScanResultMetadata* metadata,
		CdbRecord* newRecord) {
	NullScanParam* param = dynamic_cast<NullScanParam*>(toValueProvider()); __STP(param);

	AbstractCdbValue* value = param->evaluate(vm, record, metadata);
	newRecord->addValue(value);
}

IValueProvider* NullLiteralColumnParam::toValueProvider() {
	return new NullScanParam();
}

} /* namespace codablecash */
