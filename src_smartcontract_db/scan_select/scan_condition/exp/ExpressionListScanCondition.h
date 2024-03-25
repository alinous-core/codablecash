/*
 * ExpressionListScanCondition.h
 *
 *  Created on: 2020/08/05
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_EXP_EXPRESSIONLISTSCANCONDITION_H_
#define SCAN_CONDITION_EXP_EXPRESSIONLISTSCANCONDITION_H_

#include "scan_select/scan_condition/exp/AbstractExpressionCondition.h"

#include "base/ArrayList.h"

namespace codablecash {

class IValueProvider;

class ExpressionListScanCondition : public AbstractExpressionCondition {
public:
	ExpressionListScanCondition();
	virtual ~ExpressionListScanCondition();

	virtual const UnicodeString* toStringCode() noexcept;

	void addElement(IValueProvider* val) noexcept;

	virtual void analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner);
	virtual void detectFilterConditions(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector) const;
	virtual void detectIndexCandidate(VirtualMachine* vm, SelectScanPlanner* planner, TableIndexDetector* detector) const;

	virtual AbstractScanCondition* cloneCondition() const noexcept;

	virtual AbstractCdbValue* evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata);

	virtual bool hasColumnId() const noexcept;
	virtual bool isRecordIndependent() const noexcept;
	virtual bool isAvailable(VirtualMachine* vm) const;
	virtual bool hasPlaceholder() const noexcept;

	const ArrayList<IValueProvider>* getList() const noexcept {
		return &list;
	}

private:
	bool isFilterable(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector) const noexcept;

	void resetStr() noexcept;

private:
	ArrayList<IValueProvider> list;

	UnicodeString* str;
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_EXP_EXPRESSIONLISTSCANCONDITION_H_ */
