/*
 * IsNullScanCondition.h
 *
 *  Created on: 2020/08/04
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_EXP_ISNULLSCANCONDITION_H_
#define SCAN_CONDITION_EXP_ISNULLSCANCONDITION_H_

#include "scan_select/scan_condition/base/AbstractScanCondition.h"

namespace codablecash {

class IValueProvider;

class IsNullScanCondition : public AbstractScanCondition {
public:
	IsNullScanCondition();
	virtual ~IsNullScanCondition();

	void setCondition(IValueProvider* cond) noexcept;
	void setIsNull(bool notnull) noexcept;

	virtual const UnicodeString* toStringCode() noexcept;

	virtual void analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner);
	virtual void detectFilterConditions(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector);
	virtual void detectIndexCondition(VirtualMachine* vm, SelectScanPlanner* planner, TableIndexDetector* detector);

	virtual AbstractScanCondition* cloneCondition() const noexcept;

private:
	void resetStr() noexcept;

private:
	IValueProvider* cond;
	bool notnull;

	UnicodeString* str;
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_EXP_ISNULLSCANCONDITION_H_ */
