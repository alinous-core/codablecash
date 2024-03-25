/*
 * AbstractScanConditionElement.h
 *
 *  Created on: 2020/07/20
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_PARAMS_ABSTRACTSCANCONDITIONPARAMETER_H_
#define SCAN_CONDITION_PARAMS_ABSTRACTSCANCONDITIONPARAMETER_H_

#include "scan_select/scan_condition/base/AbstractScanConditionElement.h"

namespace alinous {
class VirtualMachine;
}  // namespace alinous


namespace codablecash {

class CdbRecord;
class AbstractCdbValue;

class AbstractScanConditionParameter : public AbstractScanConditionElement {
public:
	AbstractScanConditionParameter();
	virtual ~AbstractScanConditionParameter();

	virtual void analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner){}
	virtual bool isFilterable(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector) const noexcept;
	virtual void detectFilterConditions(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector);
	virtual bool isRecordIndependent() const noexcept;

	virtual bool isAvailable(VirtualMachine* vm) const {return true;};
	virtual bool hasPlaceholder() const noexcept {return false;};
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_PARAMS_ABSTRACTSCANCONDITIONPARAMETER_H_ */
