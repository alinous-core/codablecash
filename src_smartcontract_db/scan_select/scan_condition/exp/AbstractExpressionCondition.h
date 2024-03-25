/*
 * AbstractExpressionCondition.h
 *
 *  Created on: 2023/06/16
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_CONDITION_EXP_ABSTRACTEXPRESSIONCONDITION_H_
#define SCAN_SELECT_SCAN_CONDITION_EXP_ABSTRACTEXPRESSIONCONDITION_H_

#include "scan_select/scan_condition/base/AbstractScanCondition.h"

namespace codablecash {

class AbstractExpressionCondition : public AbstractScanCondition {
public:
	explicit AbstractExpressionCondition(short type);
	virtual ~AbstractExpressionCondition();

	virtual bool isFilterable(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector) const noexcept;

};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_CONDITION_EXP_ABSTRACTEXPRESSIONCONDITION_H_ */
