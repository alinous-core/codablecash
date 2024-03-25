/*
 * NotScanColumnTarget.cpp
 *
 *  Created on: 2020/08/21
 *      Author: iizuka
 */

#include "scan_select/scan_columns/scan_columns_logical/NotScanColumnTarget.h"

#include "base/UnicodeString.h"

#include "scan_select/scan_condition/logical/NotScanCondition.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"
#include "schema_table/record/table_record/CdbRecord.h"

#include "base/StackRelease.h"


namespace codablecash {

NotScanColumnTarget::NotScanColumnTarget() {
	this->cond = nullptr;
	this->notCondition = nullptr;
}

NotScanColumnTarget::~NotScanColumnTarget() {
	delete this->cond;
	delete this->notCondition;
}

void NotScanColumnTarget::setCond(AbstractScanColumnsTarget* cond) noexcept {
	this->cond = cond;
}

const UnicodeString* NotScanColumnTarget::toStringCode() noexcept {
	if(this->str == nullptr){
		this->str = new UnicodeString(L"");

		this->str->append(L"NOT ");
		this->str->append(this->cond->toStringCode());
	}

	return this->str;
}

void NotScanColumnTarget::resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner) {
	this->cond->resolveColumns(vm, planner);

	this->notCondition = new NotScanCondition();
	this->notCondition->addCondition(dynamic_cast<AbstractScanConditionElement*>(this->cond->toValueProvider()));
}

void NotScanColumnTarget::scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord) {
	NotScanCondition* cnd = dynamic_cast<NotScanCondition*>(toValueProvider()); __STP(cnd);

	AbstractCdbValue* value = cnd->evaluate(vm, record, metadata);
	newRecord->addValue(value);
}

IValueProvider* NotScanColumnTarget::toValueProvider() {
	return this->notCondition->cloneCondition();
}

} /* namespace codablecash */
