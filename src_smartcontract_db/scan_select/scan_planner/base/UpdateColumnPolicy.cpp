/*
 * UpdateColumnPolicy.cpp
 *
 *  Created on: 2022/01/08
 *      Author: iizuka
 */

#include "scan_select/scan_planner/base/UpdateColumnPolicy.h"

#include "scan_select/scan_condition/base/AbstractScanConditionElement.h"

#include "trx/scan/transaction_scan_result/ScanResultFieldMetadata.h"

#include "schema_table/record/table_record/CdbRecord.h"

namespace codablecash {

UpdateColumnPolicy::UpdateColumnPolicy() {
	this->fld = nullptr;
	this->valueProvider = nullptr;
}

UpdateColumnPolicy::~UpdateColumnPolicy() {
	delete this->fld;
	delete this->valueProvider;
}

void UpdateColumnPolicy::setFieldMetadata(ScanResultFieldMetadata* fld) noexcept {
	this->fld = fld;
}

void UpdateColumnPolicy::setValueProvider(AbstractScanConditionElement* valueProvider) noexcept {
	this->valueProvider = valueProvider;
}

void UpdateColumnPolicy::updateRecord(VirtualMachine* vm, CdbRecord* record, const ScanResultMetadata* metadata) {
	AbstractCdbValue* value = this->valueProvider->evaluate(vm, record, metadata);

	int pos = this->fld->getPosition();
	record->setValue(value, pos);
}

} /* namespace codablecash */
