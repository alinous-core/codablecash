/*
 * AdditiveScanCondition.h
 *
 *  Created on: 2020/07/31
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_ARITHMETIC_ADDITIVESCANCONDITION_H_
#define SCAN_CONDITION_ARITHMETIC_ADDITIVESCANCONDITION_H_

#include "scan_select/scan_condition/arithmetic/AbstractArithmeticCondition.h"

using namespace alinous;

namespace codablecash {

class AdditiveScanCondition : public AbstractArithmeticCondition {
public:
	AdditiveScanCondition();
	virtual ~AdditiveScanCondition();

	virtual const UnicodeString* toStringCode() noexcept;

	virtual IValueProvider* clone() const noexcept;

	virtual AbstractCdbValue* evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata);

};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_ARITHMETIC_ADDITIVESCANCONDITION_H_ */
