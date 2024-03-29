/*
 * BooleanScanParam.h
 *
 *  Created on: 2020/07/27
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_PARAMS_BOOLEANSCANPARAM_H_
#define SCAN_CONDITION_PARAMS_BOOLEANSCANPARAM_H_

namespace alinous {
class UnicodeString;
}
using namespace alinous;

#include "scan_select/scan_condition/params/AbstractScanConditionParameter.h"

namespace codablecash {

class BooleanScanParam : public AbstractScanConditionParameter {
public:
	static const UnicodeString TRUE;
	static const UnicodeString FALSE;

	BooleanScanParam(const BooleanScanParam& inst);
	explicit BooleanScanParam(bool value);
	virtual ~BooleanScanParam();

	virtual const UnicodeString* toStringCode() noexcept;
	virtual IValueProvider* clone() const noexcept;

	virtual AbstractCdbValue* evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata);
private:
	bool value;
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_PARAMS_BOOLEANSCANPARAM_H_ */
