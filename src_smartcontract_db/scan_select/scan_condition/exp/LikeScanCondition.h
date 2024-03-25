/*
 * LikeScanCondition.h
 *
 *  Created on: 2020/08/05
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_EXP_LIKESCANCONDITION_H_
#define SCAN_CONDITION_EXP_LIKESCANCONDITION_H_

#include "scan_select/scan_condition/exp/AbstractExpressionCondition.h"

namespace codablecash {

class IValueProvider;
class LikeExpressionProcessor;

class LikeScanCondition : public AbstractExpressionCondition {
public:
	LikeScanCondition();
	virtual ~LikeScanCondition();

	void setLeft(IValueProvider* left) noexcept;
	void setRight(IValueProvider* right) noexcept;
	void setEscape(IValueProvider* escape) noexcept;

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
	IValueProvider* left;
	IValueProvider* right;
	IValueProvider* escape;

	UnicodeString* str;

	LikeExpressionProcessor* processor;
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_EXP_LIKESCANCONDITION_H_ */
