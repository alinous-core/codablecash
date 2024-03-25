/*
 * EqualityScanCondition.h
 *
 *  Created on: 2020/07/20
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_EQUALITYSCANCONDITION_H_
#define SCAN_CONDITION_EQUALITYSCANCONDITION_H_

#include "scan_select/scan_condition/exp/AbstractExpressionCondition.h"

namespace codablecash {

class IValueProvider;
class AbstractScanTableTarget;
class ColumnIdentifierScanParam;

class EqualityScanCondition : public AbstractExpressionCondition {
public:
	EqualityScanCondition();
	virtual ~EqualityScanCondition();

	void setLeft(IValueProvider* element);
	void setRight(IValueProvider* element);

	virtual const UnicodeString* toStringCode() noexcept;

	virtual void analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner);
	virtual void collectJoinCandidate(VirtualMachine* vm, SelectScanPlanner* planner, int joinType, JoinCandidateHolder* jholder);
	virtual void detectFilterConditions(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector) const;
	virtual void detectIndexCandidate(VirtualMachine* vm, SelectScanPlanner* planner, TableIndexDetector* detector) const;

	virtual AbstractScanCondition* cloneCondition() const noexcept;

	virtual AbstractCdbValue* evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata);
	virtual bool isRecordIndependent() const noexcept;
	virtual bool isAvailable(VirtualMachine* vm) const;
	virtual bool hasPlaceholder() const noexcept;

	IValueProvider* getLeft() const noexcept {
		return this->left;
	}
	IValueProvider* getRight() const noexcept {
		return this->right;
	}

private:
	bool hasLeftAndRightScanTarget(const AbstractScanTableTarget* left, const AbstractScanTableTarget* right) const noexcept;
	ColumnIdentifierScanParam* getTargetCond(const AbstractScanTableTarget* target);

	void resetStr() noexcept;
private:
	IValueProvider* left;
	IValueProvider* right;

	UnicodeString* str;
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_EQUALITYSCANCONDITION_H_ */
