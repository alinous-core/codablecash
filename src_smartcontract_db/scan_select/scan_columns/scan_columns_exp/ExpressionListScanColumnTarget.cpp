/*
 * ExpressionListScanColumnTarget.cpp
 *
 *  Created on: 2020/08/21
 *      Author: iizuka
 */

#include "scan_select/scan_columns/scan_columns_exp/ExpressionListScanColumnTarget.h"

#include "base/UnicodeString.h"

#include "scan_select/scan_condition/exp/ExpressionListScanCondition.h"

#include "base/StackRelease.h"

#include "schema_table/record/table_record/CdbRecord.h"
namespace codablecash {

ExpressionListScanColumnTarget::ExpressionListScanColumnTarget() {
	this->cond = nullptr;
}

ExpressionListScanColumnTarget::~ExpressionListScanColumnTarget() {
	this->list.deleteElements();
	delete this->cond;
}

void ExpressionListScanColumnTarget::addElement(AbstractScanColumnsTarget* val) noexcept {
	this->list.addElement(val);
}

const UnicodeString* ExpressionListScanColumnTarget::toStringCode() noexcept {
	if(this->str == nullptr){
		this->str = new UnicodeString(L"");

		int maxLoop = this->list.size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractScanColumnsTarget* vp = this->list.get(i);

			if(i > 0){
				this->str->append(L", ");
			}

			this->str->append(vp->toStringCode());
		}
	}

	return this->str;
}

void ExpressionListScanColumnTarget::resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner) {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanColumnsTarget* vp = this->list.get(i);

		vp->resolveColumns(vm, planner);
	}

	this->cond = new ExpressionListScanCondition();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanColumnsTarget* vp = this->list.get(i);

		this->cond->addElement(vp->toValueProvider());
	}
}

void ExpressionListScanColumnTarget::scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord) {
	ExpressionListScanCondition* cnd = dynamic_cast<ExpressionListScanCondition*>(toValueProvider()); __STP(cnd);

	AbstractCdbValue* value = cnd->evaluate(vm, record, metadata);
	newRecord->addValue(value);
}

IValueProvider* ExpressionListScanColumnTarget::toValueProvider() {
	return this->cond->cloneCondition();
}

} /* namespace codablecash */
