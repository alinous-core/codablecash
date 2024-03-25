/*
 * IsNullScanColumnTarget.cpp
 *
 *  Created on: 2020/08/21
 *      Author: iizuka
 */

#include "scan_select/scan_columns/scan_columns_exp/IsNullScanColumnTarget.h"

#include "base/UnicodeString.h"

#include "scan_select/scan_condition/exp/IsNullScanCondition.h"

#include "base/StackRelease.h"

#include "schema_table/record/table_record/CdbRecord.h"
namespace codablecash {

IsNullScanColumnTarget::IsNullScanColumnTarget() {
	this->innerCond = nullptr;
	this->notnull = false;
	this->cond = nullptr;
}

IsNullScanColumnTarget::~IsNullScanColumnTarget() {
	delete this->innerCond;
	delete this->cond;
}

void IsNullScanColumnTarget::setCondition(AbstractScanColumnsTarget* cond) noexcept {
	this->innerCond = cond;
}

void IsNullScanColumnTarget::setIsNull(bool notnull) noexcept {
	this->notnull = notnull;
}

const UnicodeString* IsNullScanColumnTarget::toStringCode() noexcept {
	if(this->str == nullptr){
		this->str = new UnicodeString(L"");

		this->str->append(this->innerCond->toStringCode());

		this->str->append(L" IS");

		if(this->notnull){
			this->str->append(L" NOT");
		}

		this->str->append(L" NULL");
	}

	return this->str;
}

void IsNullScanColumnTarget::resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner) {
	this->innerCond->resolveColumns(vm, planner);

	this->cond = new IsNullScanCondition();
	this->cond->setIsNull(this->notnull);
	this->cond->setCondition(this->innerCond->toValueProvider());
}

void IsNullScanColumnTarget::scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord) {
	IsNullScanCondition* cnd = dynamic_cast<IsNullScanCondition*>(toValueProvider()); __STP(cnd);

	AbstractCdbValue* value = cnd->evaluate(vm, record, metadata);
	newRecord->addValue(value);
}

IValueProvider* IsNullScanColumnTarget::toValueProvider() {
	return this->cond->cloneCondition();
}

} /* namespace codablecash */
