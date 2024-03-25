/*
 * NullScanParam.h
 *
 *  Created on: 2021/01/13
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_CONDITION_PARAMS_NULLSCANPARAM_H_
#define SCAN_SELECT_SCAN_CONDITION_PARAMS_NULLSCANPARAM_H_

#include "scan_select/scan_condition/params/AbstractScanConditionParameter.h"

#include "base/UnicodeString.h"

namespace codablecash {

class NullScanParam : public AbstractScanConditionParameter {
public:
	static const UnicodeString NULL_STR;

	NullScanParam(const NullScanParam& inst);
	NullScanParam();
	virtual ~NullScanParam();

	virtual const UnicodeString* toStringCode() noexcept;
	virtual IValueProvider* clone() const noexcept;

	virtual AbstractCdbValue* evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata);
private:

};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_CONDITION_PARAMS_NULLSCANPARAM_H_ */
