/*
 * AbstractArithmeticCondition.cpp
 *
 *  Created on: 2023/06/15
 *      Author: iizuka
 */

#include "scan_select/scan_condition/arithmetic/AbstractArithmeticCondition.h"

#include "scan_select/scan_condition/base/IValueProvider.h"

#include "base/UnicodeString.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionStackMarker.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"
namespace codablecash {

AbstractArithmeticCondition::AbstractArithmeticCondition(short type)  : AbstractScanCondition(type), operations(2) {
	this->str = nullptr;
}

AbstractArithmeticCondition::~AbstractArithmeticCondition() {
	this->list.deleteElements();
	resetStr();
}

bool AbstractArithmeticCondition::hasColumnId() const noexcept {
	bool result = false;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanConditionElement* vp = dynamic_cast<AbstractScanConditionElement*>(this->list.get(i));

		if(vp->hasColumnId()){
			result = true;
			break;
		}
	}

	return result;
}

void AbstractArithmeticCondition::addOperand(AbstractScanConditionElement *vp) noexcept {
	this->list.addElement(vp);
	resetStr();
}

void AbstractArithmeticCondition::addOperator(uint8_t op) noexcept {
	this->operations.addElement(op);
	resetStr();
}

bool AbstractArithmeticCondition::isRecordIndependent() const noexcept {
	int ret = true;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		IValueProvider* vp = this->list.get(i);

		if(!vp->isRecordIndependent()){
			ret = false;
			break;
		}
	}

	return ret;
}

bool AbstractArithmeticCondition::isAvailable(VirtualMachine *vm) const {
	int ret = true;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		IValueProvider* vp = this->list.get(i);

		if(!vp->isAvailable(vm)){
			ret = false;
			break;
		}
	}

	return ret;
}

bool AbstractArithmeticCondition::hasPlaceholder() const noexcept {
	int ret = false;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		IValueProvider* vp = this->list.get(i);

		if(vp->hasPlaceholder()){
			ret = true;
			break;
		}
	}

	return ret;
}

bool AbstractArithmeticCondition::isFilterable(VirtualMachine *vm, SelectScanPlanner *planner, FilterConditionDitector *detector) const noexcept {
	FilterConditionStackMarker marker(detector->getStack());

	bool result = true;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		IValueProvider* vp = this->list.get(i);

		if(!vp->isFilterable(vm, planner, detector)){
			result = false;
			break;
		}
	}

	return result;
}

void AbstractArithmeticCondition::detectFilterConditions(VirtualMachine *vm, SelectScanPlanner *planner, FilterConditionDitector *detector) const {
	if(isFilterable(vm, planner, detector)){
		detector->push(cloneCondition());
	}
}

void AbstractArithmeticCondition::detectIndexCandidate(VirtualMachine *vm, SelectScanPlanner *planner, TableIndexDetector *detector) const {
}

void AbstractArithmeticCondition::analyzeConditions(VirtualMachine *vm, SelectScanPlanner *planner) {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		IValueProvider* vp = this->list.get(i);

		vp->analyzeConditions(vm, planner);
	}
}

AbstractScanCondition* AbstractArithmeticCondition::cloneCondition() const noexcept {
	return dynamic_cast<AbstractScanCondition*>(clone());
}

void AbstractArithmeticCondition::resetStr() noexcept {
	if(this->str != nullptr){
		delete this->str;
		this->str = nullptr;
	}
}

} /* namespace codablecash */
