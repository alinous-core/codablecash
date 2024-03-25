/*
 * OrlScanColumnTarget.cpp
 *
 *  Created on: 2020/08/21
 *      Author: iizuka
 */

#include "scan_select/scan_columns/scan_columns_logical/OrScanColumnTarget.h"

#include "base/UnicodeString.h"

#include "scan_select/scan_condition/logical/OrScanCondition.h"

#include "base/StackRelease.h"

#include "schema_table/record/table_record/CdbRecord.h"
namespace codablecash {

OrScanColumnTarget::OrScanColumnTarget() {
	this->cond = nullptr;
}

OrScanColumnTarget::~OrScanColumnTarget() {
	this->list.deleteElements();
	delete this->cond;
}

void OrScanColumnTarget::addCondition(AbstractScanColumnsTarget* cond) noexcept {
	this->list.addElement(cond);
}

const UnicodeString* OrScanColumnTarget::toStringCode() noexcept {
	if(this->str == nullptr){
		this->str = new UnicodeString(L"");

		int maxLoop = this->list.size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractScanColumnsTarget* cond = this->list.get(i);

			if(i != 0){
				this->str->append(L" OR ");
			}

			this->str->append(cond->toStringCode());
		}
	}

	return this->str;
}

void OrScanColumnTarget::resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner) {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanColumnsTarget* cond = this->list.get(i);

		cond->resolveColumns(vm, planner);
	}

	this->cond = new OrScanCondition();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanColumnsTarget* cond = this->list.get(i);

		this->cond->addCondition(dynamic_cast<AbstractScanConditionElement*>(cond->toValueProvider()));
	}
}

void OrScanColumnTarget::scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord) {
	OrScanCondition* cnd = dynamic_cast<OrScanCondition*>(toValueProvider()); __STP(cnd);

	AbstractCdbValue* value = cnd->evaluate(vm, record, metadata);
	newRecord->addValue(value);
}

IValueProvider* OrScanColumnTarget::toValueProvider() {
	return this->cond->cloneCondition();
}

} /* namespace codablecash */
