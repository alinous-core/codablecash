/*
 * RootScanCondition.h
 *
 *  Created on: 2020/06/14
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_ROOTSCANCONDITION_H_
#define SCAN_CONDITION_ROOTSCANCONDITION_H_

#include "scan_select/scan_condition/base/AbstractScanCondition.h"

namespace codablecash {

class RootScanCondition : public AbstractScanCondition {
public:
	RootScanCondition();
	virtual ~RootScanCondition();

	virtual void addCondition(AbstractScanCondition* cond);
	virtual bool isContainer() const noexcept { return true; };

	virtual const UnicodeString* toStringCode() noexcept;

	virtual void analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner);

	virtual bool isFilterable(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector) const noexcept;
	virtual void detectFilterConditions(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector) const;
	virtual void detectIndexCandidate(VirtualMachine* vm, SelectScanPlanner* planner, TableIndexDetector* detector) const;
	virtual void collectJoinCandidate(VirtualMachine* vm, SelectScanPlanner* planner, int joinType, JoinCandidateHolder* jholder);

	virtual AbstractScanCondition* cloneCondition() const noexcept;

	virtual AbstractCdbValue* evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata);
	virtual bool isRecordIndependent() const noexcept;
	virtual bool isAvailable(VirtualMachine* vm) const;
	virtual bool hasPlaceholder() const noexcept;

	bool hasCondition() const noexcept;

	AbstractScanCondition* getCondition() const noexcept {
		return this->cond;
	}

private:
	AbstractScanCondition* cond;
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_ROOTSCANCONDITION_H_ */
