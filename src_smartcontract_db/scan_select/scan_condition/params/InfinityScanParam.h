/*
 * InfinityScanParam.h
 *
 *  Created on: 2022/01/01
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_CONDITION_PARAMS_INFINITYSCANPARAM_H_
#define SCAN_SELECT_SCAN_CONDITION_PARAMS_INFINITYSCANPARAM_H_

#include "scan_select/scan_condition/params/AbstractScanConditionParameter.h"

#include "base/UnicodeString.h"

namespace codablecash {

class InfinityScanParam : public AbstractScanConditionParameter {
public:
	static const UnicodeString INFINITY_STR;

	InfinityScanParam(const InfinityScanParam& inst);
	InfinityScanParam();
	virtual ~InfinityScanParam();

	virtual const UnicodeString* toStringCode() noexcept;
	virtual IValueProvider* clone() const noexcept;

	virtual AbstractCdbValue* evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata);
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_CONDITION_PARAMS_INFINITYSCANPARAM_H_ */
