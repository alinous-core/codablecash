/*
 * RootScanCondition.cpp
 *
 *  Created on: 2020/06/14
 *      Author: iizuka
 */

#include "scan_select/scan_condition/base/RootScanCondition.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionStackMarker.h"
#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"

#include "schema_table/record/table_record_value/CdbByteValue.h"

#include "scan_select/scan_planner/scanner/index/TableIndexDetectorStack.h"
#include "scan_select/scan_planner/scanner/index/TableIndexDetector.h"
#include "scan_select/scan_planner/scanner/index/TableIndexDetectorStackMarker.h"

#include "scan_select/scan_planner/scanner/join/JoinCandidateStackMarker.h"
#include "scan_select/scan_planner/scanner/join/JoinCandidateHolder.h"

namespace codablecash {

RootScanCondition::RootScanCondition() : AbstractScanCondition(0) {
	this->cond = nullptr;
}

RootScanCondition::~RootScanCondition() {
	delete this->cond;
}

void RootScanCondition::addCondition(AbstractScanCondition* cond) {
	delete this->cond;
	this->cond = cond;
}

const UnicodeString* RootScanCondition::toStringCode() noexcept {
	return this->cond->toStringCode();
}

void RootScanCondition::analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner) {
	if(this->cond != nullptr){
		this->cond->analyzeConditions(vm, planner);
	}
}

void RootScanCondition::detectFilterConditions(VirtualMachine* vm,
		SelectScanPlanner* planner, FilterConditionDitector* detector) const {
	FilterConditionStackMarker marker(detector->getStack());

	if(this->cond != nullptr){
		this->cond->detectFilterConditions(vm, planner, detector);
	}
}

void RootScanCondition::detectIndexCandidate(VirtualMachine* vm,
		SelectScanPlanner* planner, TableIndexDetector* detector) const {
	TableIndexDetectorStack* stack = detector->getStack();
	TableIndexDetectorStackMarker marker(stack);

	if(this->cond != nullptr){
		this->cond->detectIndexCandidate(vm, planner, detector);
	}
}

void RootScanCondition::collectJoinCandidate(VirtualMachine* vm,
		SelectScanPlanner* planner, int joinType,
		JoinCandidateHolder* jholder) {
	JoinCandidateStackMarker marker(jholder->getStack());

	if(this->cond != nullptr){
		this->cond->collectJoinCandidate(vm, planner, joinType, jholder);
	}
}

AbstractScanCondition* RootScanCondition::cloneCondition() const noexcept {
	RootScanCondition* cond = new RootScanCondition();

	cond->addCondition(this->cond->cloneCondition());

	return cond;
}

AbstractCdbValue* RootScanCondition::evaluate(VirtualMachine* vm,
		const CdbRecord* record, const ScanResultMetadata* metadata) {
	if(this->cond == nullptr || !isAvailable(vm)){
		return new CdbByteValue(1);
	}

	return this->cond->evaluate(vm, record, metadata);
}

bool RootScanCondition::isFilterable(VirtualMachine *vm, SelectScanPlanner *planner, FilterConditionDitector *detector) const noexcept {
	return false;
}

bool RootScanCondition::hasCondition() const noexcept {
	return this->cond != nullptr;
}

bool RootScanCondition::isRecordIndependent() const noexcept {
	return this->cond != nullptr ? this->cond->isRecordIndependent() : true;
}

bool RootScanCondition::isAvailable(VirtualMachine *vm) const {
	return this->cond != nullptr ? this->cond->isAvailable(vm) : true;
}

bool RootScanCondition::hasPlaceholder() const noexcept {
	return this->cond != nullptr ? this->cond->hasPlaceholder() : false;
}


} /* namespace codablecash */
