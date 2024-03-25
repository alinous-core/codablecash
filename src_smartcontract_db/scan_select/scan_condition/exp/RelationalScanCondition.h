/*
 * RelationalScanCondition.h
 *
 *  Created on: 2020/07/29
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_EXP_RELATIONALSCANCONDITION_H_
#define SCAN_CONDITION_EXP_RELATIONALSCANCONDITION_H_

#include "scan_select/scan_condition/exp/AbstractExpressionCondition.h"

#include <cstdint>

#include "scan_select/scan_planner/scanner/index/AbstractIndexCandidate.h"


namespace codablecash {

class IValueProvider;
class AbstractIndexCandidate;

class RelationalScanCondition : public AbstractExpressionCondition {
public:
	RelationalScanCondition();
	virtual ~RelationalScanCondition();

	void setLeft(IValueProvider* element);
	void setRight(IValueProvider* element);
	void setOp(uint8_t op) noexcept;

	virtual const UnicodeString* toStringCode() noexcept;

	virtual void analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner);
	virtual void detectFilterConditions(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector) const;
	virtual void detectIndexCandidate(VirtualMachine* vm, SelectScanPlanner* planner, TableIndexDetector* detector) const;

	virtual AbstractScanCondition* cloneCondition() const noexcept;

	static AbstractIndexCandidate::IndexType toIndexType(uint8_t op, bool reverse);

	virtual AbstractCdbValue* evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata);
	virtual bool isRecordIndependent() const noexcept;
	virtual bool isAvailable(VirtualMachine* vm) const;
	virtual bool hasPlaceholder() const noexcept;

private:
	void resetStr() noexcept;

private:
	IValueProvider* left;
	IValueProvider* right;

	uint8_t op;

	UnicodeString* str;
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_EXP_RELATIONALSCANCONDITION_H_ */
