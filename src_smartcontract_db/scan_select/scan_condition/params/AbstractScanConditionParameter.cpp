/*
 * AbstractScanConditionElement.cpp
 *
 *  Created on: 2020/07/20
 *      Author: iizuka
 */

#include "scan_select/scan_condition/params/AbstractScanConditionParameter.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionStackMarker.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"

#include "scan_select/scan_condition/base/AbstractScanCondition.h"
namespace codablecash {

AbstractScanConditionParameter::AbstractScanConditionParameter() {

}

AbstractScanConditionParameter::~AbstractScanConditionParameter() {

}

bool AbstractScanConditionParameter::isFilterable(VirtualMachine* vm, SelectScanPlanner* planner,
		FilterConditionDitector* detector) const noexcept {
	return true;
}

void AbstractScanConditionParameter::detectFilterConditions(VirtualMachine *vm,	SelectScanPlanner *planner, FilterConditionDitector *detector) {
	FilterConditionStackMarker marker(detector->getStack());

	if(isFilterable(vm, planner, detector)){
		detector->push(dynamic_cast<AbstractScanCondition*>(clone()));
	}
}

bool AbstractScanConditionParameter::isRecordIndependent() const noexcept {
	return true;
}

} /* namespace codablecash */
