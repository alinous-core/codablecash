/*
 * ParenthesisScanCondition.cpp
 *
 *  Created on: 2020/07/30
 *      Author: iizuka
 */

#include "scan_select/scan_condition/exp/ParenthesisScanCondition.h"

#include "engine/sc/CodeElement.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"
#include "scan_select/scan_planner/scanner/ctx/FilterConditionStackMarker.h"


using namespace alinous;

namespace codablecash {

ParenthesisScanCondition::ParenthesisScanCondition() : AbstractExpressionCondition(CodeElement::SQL_EXP_NOT) {
	this->cond = nullptr;
	this->str = nullptr;
}

ParenthesisScanCondition::~ParenthesisScanCondition() {
	delete this->cond;
	resetStr();
}

void ParenthesisScanCondition::addCondition(AbstractScanConditionElement* cond) {
	delete this->cond;
	this->cond = cond;
}

const UnicodeString* ParenthesisScanCondition::toStringCode() noexcept {
	if(this->str == nullptr){
		resetStr();
		this->str = new UnicodeString(L"(");

		this->str->append(this->cond->toStringCode());

		this->str->append(L")");
	}

	return this->str;
}

void ParenthesisScanCondition::collectJoinCandidate(VirtualMachine* vm,
		SelectScanPlanner* planner, int joinType,
		JoinCandidateHolder* jholder) {
	AbstractScanCondition* scanCond = dynamic_cast<AbstractScanCondition*>(this->cond);

	if(scanCond != nullptr){
		scanCond->collectJoinCandidate(vm, planner, joinType, jholder);
	}
}

AbstractScanCondition* ParenthesisScanCondition::cloneCondition() const noexcept {
	ParenthesisScanCondition* cond = new ParenthesisScanCondition();

	cond->addCondition(dynamic_cast<AbstractScanConditionElement*>(this->cond->clone()));

	return cond;
}

void ParenthesisScanCondition::detectFilterConditions(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector) const {
	FilterConditionStackMarker marker(detector->getStack());

	AbstractScanCondition* scanCond = dynamic_cast<AbstractScanCondition*>(this->cond);
	if(scanCond != nullptr){
		scanCond->detectFilterConditions(vm, planner, detector);

		if(!detector->isEmpty()){
			AbstractScanCondition* inner = detector->pop();

			ParenthesisScanCondition* newP = new ParenthesisScanCondition();
			newP->addCondition(inner);

			detector->push(newP);
		}
	}
}

void ParenthesisScanCondition::detectIndexCandidate(VirtualMachine* vm, SelectScanPlanner* planner,
		TableIndexDetector* detector) const {
	AbstractScanCondition* scanCond = dynamic_cast<AbstractScanCondition*>(this->cond);
	if(scanCond != nullptr){
		scanCond->detectIndexCandidate(vm, planner, detector);
	}
}

void ParenthesisScanCondition::resetStr() noexcept {
	if(this->str != nullptr){
		delete this->str;
		this->str = nullptr;
	}
}

void ParenthesisScanCondition::analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner) {
	this->cond->analyzeConditions(vm, planner);
}

AbstractCdbValue* ParenthesisScanCondition::evaluate(VirtualMachine* vm, const CdbRecord* record,
		const ScanResultMetadata* metadata) {
	return this->cond->evaluate(vm, record, metadata);
}

bool ParenthesisScanCondition::isRecordIndependent() const noexcept {
	return this->cond->isRecordIndependent();
}

bool ParenthesisScanCondition::isAvailable(VirtualMachine *vm) const {
	return this->cond->isAvailable(vm);
}

bool ParenthesisScanCondition::hasPlaceholder() const noexcept {
	return this->cond->hasPlaceholder();
}

} /* namespace codablecash */
