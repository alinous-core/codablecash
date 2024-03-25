/*
 * NotScanCondition.cpp
 *
 *  Created on: 2020/07/29
 *      Author: iizuka
 */

#include "scan_select/scan_condition/logical/NotScanCondition.h"

#include "engine/sc/CodeElement.h"

#include "base/UnicodeString.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"
#include "scan_select/scan_planner/scanner/ctx/FilterConditionStackMarker.h"

#include "base/StackRelease.h"

#include "schema_table/record/table_record_value/CdbBooleanValue.h"
#include "schema_table/record/table_record_value/CdbValueCaster.h"

using namespace alinous;

namespace codablecash {

NotScanCondition::NotScanCondition() : AbstractLogicalCondition(CodeElement::SQL_EXP_NOT) {
	this->cond = nullptr;
	this->str = nullptr;
}

NotScanCondition::~NotScanCondition() {
	delete this->cond;
	resetStr();
}

bool NotScanCondition::isContainer() const noexcept {
	return true;
}

void NotScanCondition::addCondition(AbstractScanConditionElement* cond) {
	delete this->cond;
	this->cond = cond;

	resetStr();
}

AbstractScanCondition* NotScanCondition::cloneCondition() const noexcept {
	NotScanCondition* cond = new NotScanCondition();

	cond->addCondition(dynamic_cast<AbstractScanConditionElement*>(this->cond->clone()));

	return cond;
}

const UnicodeString* NotScanCondition::toStringCode() noexcept {
	if(this->str == nullptr){
		resetStr();

		this->str = new UnicodeString(L"NOT ");
		this->str->append(this->cond->toStringCode());
	}

	return this->str;
}

void NotScanCondition::detectFilterConditions(VirtualMachine* vm,
		SelectScanPlanner* planner, FilterConditionDitector* detector) const {
	FilterConditionStackMarker marker(detector->getStack());

	AbstractScanCondition* scanCond = dynamic_cast<AbstractScanCondition*>(this->cond);
	if(scanCond != nullptr){
		scanCond->detectFilterConditions(vm, planner, detector);

		if(!detector->isEmpty()){
			AbstractScanCondition* inner = detector->pop();

			NotScanCondition* newNot = new NotScanCondition();
			newNot->addCondition(inner);

			detector->push(newNot);
		}
	}
}

void NotScanCondition::detectIndexCandidate(VirtualMachine* vm, SelectScanPlanner* planner, TableIndexDetector* detector) const {
}

void NotScanCondition::resetStr() noexcept {
	if(this->str != nullptr){
		delete this->str;
		this->str = nullptr;
	}
}

AbstractCdbValue* NotScanCondition::evaluate(VirtualMachine* vm,
		const CdbRecord* record, const ScanResultMetadata* metadata) {
	AbstractCdbValue* value = this->cond->evaluate(vm, record, metadata); __STP(value);
	AbstractCdbValue* bl = CdbValueCaster::cast(value, AbstractCdbValue::TYPE_BOOLEAN); __STP(bl);

	CdbBooleanValue* blValue = dynamic_cast<CdbBooleanValue*>(bl);
	bool b = blValue->getValue();

	return new CdbBooleanValue(!b);
}

bool NotScanCondition::isRecordIndependent() const noexcept {
	return this->cond->isRecordIndependent();
}

bool NotScanCondition::isAvailable(VirtualMachine *vm) const {
	return this->cond->isAvailable(vm);
}

bool NotScanCondition::hasPlaceholder() const noexcept {
	return this->cond->hasPlaceholder();
}

} /* namespace codablecash */
