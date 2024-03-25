/*
 * MultiplicativeScanCondition.h
 *
 *  Created on: 2020/08/04
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_ARITHMETIC_MULTIPLICATIVESCANCONDITION_H_
#define SCAN_CONDITION_ARITHMETIC_MULTIPLICATIVESCANCONDITION_H_

#include "scan_select/scan_condition/arithmetic/AbstractArithmeticCondition.h"

namespace codablecash {

class MultiplicativeScanCondition : public AbstractArithmeticCondition {
public:
	MultiplicativeScanCondition();
	virtual ~MultiplicativeScanCondition();

	virtual const UnicodeString* toStringCode() noexcept;

	virtual IValueProvider* clone() const noexcept;

	virtual AbstractCdbValue* evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata);
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_ARITHMETIC_MULTIPLICATIVESCANCONDITION_H_ */
