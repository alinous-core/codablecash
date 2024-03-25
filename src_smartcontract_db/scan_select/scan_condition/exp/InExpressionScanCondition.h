/*
 * InExpressionScanCondition.h
 *
 *  Created on: 2020/08/05
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_EXP_INEXPRESSIONSCANCONDITION_H_
#define SCAN_CONDITION_EXP_INEXPRESSIONSCANCONDITION_H_

#include "scan_select/scan_condition/exp/AbstractExpressionCondition.h"

namespace codablecash {

class IValueProvider;
class ExpressionListScanCondition;

class InExpressionScanCondition : public AbstractExpressionCondition {
public:
	InExpressionScanCondition();
	virtual ~InExpressionScanCondition();

	virtual const UnicodeString* toStringCode() noexcept;

	void setLeft(IValueProvider* cond) noexcept;
	void setList(ExpressionListScanCondition* list) noexcept;

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
	IValueProvider* left;
	ExpressionListScanCondition* list;

	UnicodeString* str;
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_EXP_INEXPRESSIONSCANCONDITION_H_ */
