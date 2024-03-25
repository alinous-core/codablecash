/*
 * BetweenScanCondition.h
 *
 *  Created on: 2020/08/05
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_EXP_BETWEENSCANCONDITION_H_
#define SCAN_CONDITION_EXP_BETWEENSCANCONDITION_H_

#include "scan_select/scan_condition/exp/AbstractExpressionCondition.h"

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class IValueProvider;

class BetweenScanCondition : public AbstractExpressionCondition {
public:
	BetweenScanCondition();
	virtual ~BetweenScanCondition();

	void setLeft(IValueProvider* left) noexcept;
	void setStart(IValueProvider* start) noexcept;
	void setEnd(IValueProvider* end) noexcept;

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
	IValueProvider* start;
	IValueProvider* end;

	UnicodeString* str;
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_EXP_BETWEENSCANCONDITION_H_ */
