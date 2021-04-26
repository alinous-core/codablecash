/*
 * AbstractScanConditionElement.h
 *
 *  Created on: 2020/07/21
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_ABSTRACTSCANCONDITIONELEMENT_H_
#define SCAN_CONDITION_ABSTRACTSCANCONDITIONELEMENT_H_

namespace alinous {
class UnicodeString;
class VirtualMachine;
}
using namespace alinous;

namespace codablecash {

class SelectScanPlanner;
class ScanResultMetadata;
class CdbRecord;
class AbstractCdbValue;

class AbstractScanConditionElement {
public:
	AbstractScanConditionElement();
	virtual ~AbstractScanConditionElement();

	virtual const UnicodeString* toStringCode() noexcept = 0;
	virtual void analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner) = 0;

	virtual AbstractCdbValue* evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata) = 0;
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_ABSTRACTSCANCONDITIONELEMENT_H_ */
