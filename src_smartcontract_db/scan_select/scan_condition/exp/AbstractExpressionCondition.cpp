/*
 * AbstractExpressionCondition.cpp
 *
 *  Created on: 2023/06/16
 *      Author: iizuka
 */

#include "scan_select/scan_condition/exp/AbstractExpressionCondition.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionStackMarker.h"
#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"

namespace codablecash {

AbstractExpressionCondition::AbstractExpressionCondition(short type) : AbstractScanCondition(type) {

}

AbstractExpressionCondition::~AbstractExpressionCondition() {

}

bool AbstractExpressionCondition::isFilterable(VirtualMachine *vm, SelectScanPlanner *planner, FilterConditionDitector *detector) const noexcept {
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
