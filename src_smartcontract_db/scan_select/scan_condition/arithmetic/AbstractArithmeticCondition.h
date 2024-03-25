/*
 * AbstractArithmeticCondition.h
 *
 *  Created on: 2023/06/15
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_CONDITION_ARITHMETIC_ABSTRACTARITHMETICCONDITION_H_
#define SCAN_SELECT_SCAN_CONDITION_ARITHMETIC_ABSTRACTARITHMETICCONDITION_H_

#include <cstdint>
#include "base/ArrayList.h"
#include "base/RawArrayPrimitive.h"

#include "scan_select/scan_condition/base/AbstractScanCondition.h"

namespace codablecash {

class AbstractArithmeticCondition : public AbstractScanCondition {
public:
	explicit AbstractArithmeticCondition(short type);
	virtual ~AbstractArithmeticCondition();

	void addOperand(AbstractScanConditionElement* vp) noexcept;
	void addOperator(uint8_t op) noexcept;

	virtual void analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner);

	virtual bool isFilterable(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector) const noexcept;
	virtual void detectFilterConditions(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector) const;

	virtual void detectIndexCandidate(VirtualMachine* vm, SelectScanPlanner* planner, TableIndexDetector* detector) const;

	virtual bool hasColumnId() const noexcept;
	virtual bool isRecordIndependent() const noexcept;
	virtual bool isAvailable(VirtualMachine* vm) const;
	virtual bool hasPlaceholder() const noexcept;

	virtual AbstractScanCondition* cloneCondition() const noexcept;

protected:
	ArrayList<AbstractScanConditionElement> list;
	RawArrayPrimitive<uint8_t> operations;

	void resetStr() noexcept;

	UnicodeString* str;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_CONDITION_ARITHMETIC_ABSTRACTARITHMETICCONDITION_H_ */
