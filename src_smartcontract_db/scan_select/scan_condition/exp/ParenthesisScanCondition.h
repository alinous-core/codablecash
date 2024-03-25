/*
 * ParenthesisScanCondition.h
 *
 *  Created on: 2020/07/30
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_EXP_PARENTHESISSCANCONDITION_H_
#define SCAN_CONDITION_EXP_PARENTHESISSCANCONDITION_H_

#include "scan_select/scan_condition/exp/AbstractExpressionCondition.h"

namespace codablecash {

class ParenthesisScanCondition : public AbstractExpressionCondition {
public:
	ParenthesisScanCondition();
	virtual ~ParenthesisScanCondition();

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
	AbstractScanConditionElement* cond;

	UnicodeString* str;
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_EXP_PARENTHESISSCANCONDITION_H_ */
