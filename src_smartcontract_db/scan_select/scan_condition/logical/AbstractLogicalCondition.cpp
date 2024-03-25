/*
 * AbstractLogicalCondition.cpp
 *
 *  Created on: 2023/06/08
 *      Author: iizuka
 */

#include "scan_select/scan_condition/logical/AbstractLogicalCondition.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionStackMarker.h"
#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"

namespace codablecash {

AbstractLogicalCondition::AbstractLogicalCondition(short type) : AbstractScanCondition(type) {

}

AbstractLogicalCondition::~AbstractLogicalCondition() {

}

bool AbstractLogicalCondition::isFilterable(VirtualMachine *vm, SelectScanPlanner *planner, FilterConditionDitector *detector) const noexcept {
	FilterConditionStackMarker marker(detector->getStack());

	detectFilterConditions(vm, planner, detector);

	bool ret = false;
	if(!detector->isEmpty()){
		delete detector->pop();
		ret = true;
	}

	return ret;
}

} /* namespace codablecash */
