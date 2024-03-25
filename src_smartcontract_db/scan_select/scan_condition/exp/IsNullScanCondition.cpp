/*
 * IsNullScanCondition.cpp
 *
 *  Created on: 2020/08/04
 *      Author: iizuka
 */

#include "scan_select/scan_condition/exp/IsNullScanCondition.h"

#include "scan_select/scan_condition/base/AbstractScanConditionElement.h"

#include "engine/sc/CodeElement.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "scan_select/scan_condition/base/IValueProvider.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"
#include "scan_select/scan_planner/scanner/ctx/FilterConditionStackMarker.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"
#include "schema_table/record/table_record_value/CdbBooleanValue.h"

#include "scan_select/scan_condition/params/ColumnIdentifierScanParam.h"
#include "scan_select/scan_condition/params/NullScanParam.h"

#include "scan_select/scan_planner/scanner/index/IndexCandidate.h"
#include "scan_select/scan_planner/scanner/index/TableIndexDetector.h"

namespace codablecash {

const NullScanParam IsNullScanCondition::PARAM_VALUE;

IsNullScanCondition::IsNullScanCondition() : AbstractExpressionCondition(CodeElement::SQL_EXP_IS_NULL) {
	this->cond = nullptr;
	this->str = nullptr;
	this->notnull = false;
}

IsNullScanCondition::~IsNullScanCondition() {
	delete this->cond;
	resetStr();
}

void IsNullScanCondition::setCondition(IValueProvider* cond) noexcept {
	this->cond = cond;
	resetStr();
}

void IsNullScanCondition::setIsNull(bool notnull) noexcept {
	this->notnull = notnull;
}

const UnicodeString* IsNullScanCondition::toStringCode() noexcept {
	if(this->str == nullptr){
		resetStr();
		this->str = new UnicodeString(L"");

		AbstractScanConditionElement* element = dynamic_cast<AbstractScanConditionElement*>(this->cond);
		this->str->append(element->toStringCode());

		this->str->append(L" IS");

		if(this->notnull){
			this->str->append(L" NOT");
		}

		this->str->append(L" NULL");
	}

	return this->str;
}

AbstractScanCondition* IsNullScanCondition::cloneCondition() const noexcept {
	IsNullScanCondition* cond = new IsNullScanCondition();

	cond->setCondition(this->cond->clone());
	cond->setIsNull(this->notnull);

	return cond;
}

void IsNullScanCondition::detectFilterConditions(VirtualMachine* vm,
		SelectScanPlanner* planner, FilterConditionDitector* detector) const {
	FilterConditionStackMarker marker(detector->getStack());

	if(this->cond->isFilterable(vm, planner, detector)){
		detector->push(cloneCondition());
	}
}

void IsNullScanCondition::detectIndexCandidate(VirtualMachine* vm,
		SelectScanPlanner* planner, TableIndexDetector* detector) const {
	if(!this->notnull && this->cond->isColumn()){
		ColumnIdentifierScanParam* column = dynamic_cast<ColumnIdentifierScanParam*>(this->cond);

		IndexCandidate* candidate = new IndexCandidate(AbstractIndexCandidate::IndexType::EQUALS);
		candidate->setColumn(column);
		candidate->setValue(&PARAM_VALUE);

		detector->push(candidate);
	}
}

void IsNullScanCondition::resetStr() noexcept {
	if(this->str != nullptr){
		delete this->str;
		this->str = nullptr;
	}
}

void IsNullScanCondition::analyzeConditions(VirtualMachine* vm,	SelectScanPlanner* planner) {
	this->cond->analyzeConditions(vm, planner);
}

AbstractCdbValue* IsNullScanCondition::evaluate(VirtualMachine* vm,
		const CdbRecord* record, const ScanResultMetadata* metadata) {
	AbstractScanConditionElement* leftElement = dynamic_cast<AbstractScanConditionElement*>(this->cond);

	AbstractCdbValue* lv = leftElement->evaluate(vm, record, metadata); __STP(lv);

	return new CdbBooleanValue(this->notnull ? !lv->isNull(): lv->isNull());
}

bool IsNullScanCondition::isRecordIndependent() const noexcept {
	return this->cond->isRecordIndependent();
}

bool IsNullScanCondition::isAvailable(VirtualMachine *vm) const {
	return this->cond->isAvailable(vm);
}

bool IsNullScanCondition::hasPlaceholder() const noexcept {
	return this->cond->hasPlaceholder();
}

} /* namespace codablecash */
