/*
 * PlaceHolderScanColumnTarget.cpp
 *
 *  Created on: 2020/08/21
 *      Author: iizuka
 */

#include "scan_select/scan_columns/scan_columns_exp/PlaceHolderScanColumnTarget.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "scan_select/scan_condition/params/PlaceHolderScanCondition.h"

#include "schema_table/record/table_record/CdbRecord.h"

namespace codablecash {

PlaceHolderScanColumnTarget::PlaceHolderScanColumnTarget(AbstractExpression* exp) {
	this->exp = exp;
	this->cond = nullptr;
}

PlaceHolderScanColumnTarget::~PlaceHolderScanColumnTarget() {
	this->exp = nullptr;
	delete this->cond;
}

const UnicodeString* PlaceHolderScanColumnTarget::toStringCode() noexcept {
	if(this->str == nullptr){
		this->str = new UnicodeString(L"${}");
	}

	return this->str;
}

void PlaceHolderScanColumnTarget::resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner) {
	this->cond = new PlaceHolderScanCondition(this->exp);
}

void PlaceHolderScanColumnTarget::scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata,	CdbRecord* newRecord) {
	PlaceHolderScanCondition* cnd = dynamic_cast<PlaceHolderScanCondition*>(toValueProvider()); __STP(cnd);

	AbstractCdbValue* value = cnd->evaluate(vm, record, metadata);
	newRecord->addValue(value);
}

IValueProvider* PlaceHolderScanColumnTarget::toValueProvider() {
	return this->cond->clone();
}

} /* namespace codablecash */
