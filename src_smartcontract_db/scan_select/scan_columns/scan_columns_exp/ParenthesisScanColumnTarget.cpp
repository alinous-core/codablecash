/*
 * ParenthesisScanColumnTarget.cpp
 *
 *  Created on: 2020/08/21
 *      Author: iizuka
 */

#include "scan_select/scan_columns/scan_columns_exp/ParenthesisScanColumnTarget.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "scan_select/scan_condition/exp/ParenthesisScanCondition.h"

#include "schema_table/record/table_record/CdbRecord.h"

namespace codablecash {

ParenthesisScanColumnTarget::ParenthesisScanColumnTarget() {
	this->innerCond = nullptr;
	this->cond = nullptr;
}

ParenthesisScanColumnTarget::~ParenthesisScanColumnTarget() {
	delete this->innerCond;
	delete this->cond;
}

void ParenthesisScanColumnTarget::setInnser(AbstractScanColumnsTarget* cond) noexcept {
	this->innerCond = cond;
}

const UnicodeString* ParenthesisScanColumnTarget::toStringCode() noexcept {
	if(this->str == nullptr){
		this->str = new UnicodeString(L"(");

		this->str->append(this->innerCond->toStringCode());

		this->str->append(L")");
	}

	return this->str;
}

void ParenthesisScanColumnTarget::resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner) {
	this->innerCond->resolveColumns(vm, planner);

	this->cond = new ParenthesisScanCondition();
	this->cond->addCondition(dynamic_cast<AbstractScanConditionElement*>(this->innerCond->toValueProvider()));
}

void ParenthesisScanColumnTarget::scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord) {
	ParenthesisScanCondition* cnd = dynamic_cast<ParenthesisScanCondition*>(toValueProvider()); __STP(cnd);

	AbstractCdbValue* value = cnd->evaluate(vm, record, metadata);
	newRecord->addValue(value);
}

IValueProvider* ParenthesisScanColumnTarget::toValueProvider() {
	return this->cond->cloneCondition();
}

} /* namespace codablecash */
