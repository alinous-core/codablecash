/*
 * AndLogicalScanColumn.cpp
 *
 *  Created on: 2020/08/20
 *      Author: iizuka
 */

#include "scan_select/scan_columns/scan_columns_logical/AndScanColumnTarget.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "scan_select/scan_condition/logical/AndScanCondition.h"

#include "schema_table/record/table_record/CdbRecord.h"

namespace codablecash {

AndScanColumnTarget::AndScanColumnTarget() {
	this->cond = nullptr;
}

AndScanColumnTarget::~AndScanColumnTarget() {
	this->list.deleteElements();
	delete this->cond;
}

void AndScanColumnTarget::addCondition(AbstractScanColumnsTarget* cond) noexcept {
	this->list.addElement(cond);
}

const UnicodeString* AndScanColumnTarget::toStringCode() noexcept {
	if(this->str == nullptr){
		this->str = new UnicodeString(L"");

		int maxLoop = this->list.size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractScanColumnsTarget* cond = this->list.get(i);

			if(i != 0){
				this->str->append(L" AND ");
			}

			this->str->append(cond->toStringCode());
		}
	}

	return this->str;
}

void AndScanColumnTarget::resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner) {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanColumnsTarget* cond = this->list.get(i);

		cond->resolveColumns(vm, planner);
	}

	this->cond = new AndScanCondition();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanColumnsTarget* cond = this->list.get(i);

		this->cond->addCondition(dynamic_cast<AbstractScanConditionElement*>(cond->toValueProvider()));
	}
}

void AndScanColumnTarget::scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord) {
	AndScanCondition* cnd = dynamic_cast<AndScanCondition*>(toValueProvider()); __STP(cnd);

	AbstractCdbValue* value = cnd->evaluate(vm, record, metadata);
	newRecord->addValue(value);
}

IValueProvider* AndScanColumnTarget::toValueProvider() {
	return this->cond->cloneCondition();
}

} /* namespace codablecash */
