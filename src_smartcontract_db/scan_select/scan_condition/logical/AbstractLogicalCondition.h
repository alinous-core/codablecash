/*
 * AbstractLogicalCondition.h
 *
 *  Created on: 2023/06/08
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_CONDITION_LOGICAL_ABSTRACTLOGICALCONDITION_H_
#define SCAN_SELECT_SCAN_CONDITION_LOGICAL_ABSTRACTLOGICALCONDITION_H_

#include "scan_select/scan_condition/base/AbstractScanCondition.h"

namespace codablecash {

class AbstractLogicalCondition : public AbstractScanCondition {
public:
	explicit AbstractLogicalCondition(short type);
	virtual ~AbstractLogicalCondition();

	virtual bool isFilterable(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector) const noexcept;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_CONDITION_LOGICAL_ABSTRACTLOGICALCONDITION_H_ */
