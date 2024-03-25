/*
 * FunctionCallScanCondition.h
 *
 *  Created on: 2020/08/05
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_EXP_FUNCTIONCALLSCANCONDITION_H_
#define SCAN_CONDITION_EXP_FUNCTIONCALLSCANCONDITION_H_

#include "scan_select/scan_condition/exp/AbstractExpressionCondition.h"

#include "base/ArrayList.h"

namespace codablecash {

class IValueProvider;

class FunctionCallScanCondition : public AbstractExpressionCondition {
public:
	FunctionCallScanCondition(const FunctionCallScanCondition& inst);
	FunctionCallScanCondition();
	virtual ~FunctionCallScanCondition();

	virtual const UnicodeString* toStringCode() noexcept;

	void setName(UnicodeString* name) noexcept;
	void addArgument(IValueProvider* arg) noexcept;

	virtual void analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner);
	virtual bool isFilterable(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector) const noexcept;
	virtual void detectFilterConditions(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector) const;
	virtual void detectIndexCandidate(VirtualMachine* vm, SelectScanPlanner* planner, TableIndexDetector* detector) const;


	virtual IValueProvider* clone() const noexcept;
	virtual AbstractScanCondition* cloneCondition() const noexcept;

	virtual AbstractCdbValue* evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata);

	virtual bool hasColumnId() const noexcept;
	virtual bool isRecordIndependent() const noexcept;
	virtual bool isAvailable(VirtualMachine* vm) const;
	virtual bool hasPlaceholder() const noexcept;

private:
	void resetStr() noexcept;

private:
	UnicodeString* name;
	ArrayList<IValueProvider> arguments;

	UnicodeString* str;
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_EXP_FUNCTIONCALLSCANCONDITION_H_ */
