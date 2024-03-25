/*
 * NotScanCondition.h
 *
 *  Created on: 2020/07/29
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_LOGICAL_NOTSCANCONDITION_H_
#define SCAN_CONDITION_LOGICAL_NOTSCANCONDITION_H_

#include "scan_select/scan_condition/logical/AbstractLogicalCondition.h"

namespace codablecash {

class NotScanCondition : public AbstractLogicalCondition {
public:
	NotScanCondition();
	virtual ~NotScanCondition();

	virtual bool isContainer() const noexcept;
	virtual void addCondition(AbstractScanConditionElement* cond);

	virtual const UnicodeString* toStringCode() noexcept;
	virtual AbstractScanCondition* cloneCondition() const noexcept;

	virtual void detectFilterConditions(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector) const;
	virtual void detectIndexCandidate(VirtualMachine* vm, SelectScanPlanner* planner, TableIndexDetector* detector) const;

	virtual AbstractCdbValue* evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata);
	virtual bool isRecordIndependent() const noexcept;

	virtual bool isAvailable(VirtualMachine* vm) const;
	virtual bool hasPlaceholder() const noexcept;

private:
	void resetStr() noexcept;

private:
	AbstractScanConditionElement* cond;
	UnicodeString* str;
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_LOGICAL_NOTSCANCONDITION_H_ */
