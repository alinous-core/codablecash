/*
 * AbstractScanCondition.h
 *
 *  Created on: 2020/06/04
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_ABSTRACTSCANCONDITION_H_
#define SCAN_CONDITION_ABSTRACTSCANCONDITION_H_

#include "scan_select/scan_condition/base/AbstractScanConditionElement.h"

namespace alinous {
class UnicodeString;
class VirtualMachine;
}  // namespace alinous

namespace codablecash {

class JoinCandidateHolder;
class FilterConditionDitector;
class TableIndexDetector;
class CdbRecord;
class ScanResultMetadata;
class AbstractCdbValue;

class AbstractScanCondition : public AbstractScanConditionElement {
public:
	explicit AbstractScanCondition(short type);
	virtual ~AbstractScanCondition();

	virtual bool isContainer() const noexcept;

	virtual void analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner){};

	virtual void collectJoinCandidate(VirtualMachine* vm, SelectScanPlanner* planner, int joinType, JoinCandidateHolder* jholder) {};
	virtual void detectFilterConditions(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector) const = 0;
	virtual void detectIndexCandidate(VirtualMachine* vm, SelectScanPlanner* planner, TableIndexDetector* detector) const = 0;

	virtual AbstractScanCondition* cloneCondition() const noexcept = 0;
	virtual IValueProvider* clone() const noexcept;

private:
	short type;
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_ABSTRACTSCANCONDITION_H_ */
