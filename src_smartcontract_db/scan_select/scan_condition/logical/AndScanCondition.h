/*
 * AndScanCondition.h
 *
 *  Created on: 2020/07/30
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_LOGICAL_ANDSCANCONDITION_H_
#define SCAN_CONDITION_LOGICAL_ANDSCANCONDITION_H_

#include "scan_select/scan_condition/logical/AbstractLogicalCondition.h"
#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class AndScanCondition : public AbstractLogicalCondition {
public:
	AndScanCondition();
	virtual ~AndScanCondition();

	virtual bool isContainer() const noexcept {return true;};
	virtual void addCondition(AbstractScanConditionElement* cond);

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

private:
	void resetStr() noexcept;

private:
	ArrayList<AbstractScanConditionElement> list;

	UnicodeString* str;
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_LOGICAL_ANDSCANCONDITION_H_ */
