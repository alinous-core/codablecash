/*
 * IsNullScanCondition.h
 *
 *  Created on: 2020/08/04
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_EXP_ISNULLSCANCONDITION_H_
#define SCAN_CONDITION_EXP_ISNULLSCANCONDITION_H_

#include "scan_select/scan_condition/exp/AbstractExpressionCondition.h"

#include "scan_select/scan_condition/params/NullScanParam.h"

namespace codablecash {

class IValueProvider;

class IsNullScanCondition : public AbstractExpressionCondition {
public:
	static const NullScanParam PARAM_VALUE;

	IsNullScanCondition();
	virtual ~IsNullScanCondition();

	void setCondition(IValueProvider* cond) noexcept;
	void setIsNull(bool notnull) noexcept;

	virtual const UnicodeString* toStringCode() noexcept;

	virtual void analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner);
	virtual void detectFilterConditions(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector) const;
	virtual void detectIndexCandidate(VirtualMachine* vm, SelectScanPlanner* planner, TableIndexDetector* detector) const;

	virtual AbstractScanCondition* cloneCondition() const noexcept;

	virtual AbstractCdbValue* evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata);
	virtual bool isRecordIndependent() const noexcept;
	virtual bool isAvailable(VirtualMachine* vm) const;
	virtual bool hasPlaceholder() const noexcept;

private:
	void resetStr() noexcept;

private:
	IValueProvider* cond;
	bool notnull;

	UnicodeString* str;
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_EXP_ISNULLSCANCONDITION_H_ */
