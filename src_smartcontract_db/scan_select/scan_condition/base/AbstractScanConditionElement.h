/*
 * AbstractScanConditionElement.h
 *
 *  Created on: 2020/07/21
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_ABSTRACTSCANCONDITIONELEMENT_H_
#define SCAN_CONDITION_ABSTRACTSCANCONDITIONELEMENT_H_

#include "scan_select/scan_condition/base/IValueProvider.h"

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

class AbstractScanConditionElement : public IValueProvider {
public:
	AbstractScanConditionElement();
	virtual ~AbstractScanConditionElement();

};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_ABSTRACTSCANCONDITIONELEMENT_H_ */
