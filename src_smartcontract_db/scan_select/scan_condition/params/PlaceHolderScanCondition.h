/*
 * PlaceHolderScanCondition.h
 *
 *  Created on: 2020/08/05
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_PARAMS_PLACEHOLDERSCANCONDITION_H_
#define SCAN_CONDITION_PARAMS_PLACEHOLDERSCANCONDITION_H_

#include "scan_select/scan_condition/params/AbstractScanConditionParameter.h"

namespace alinous {
class AbstractExpression;
}
using namespace alinous;

namespace codablecash {

class PlaceHolderScanCondition : public AbstractScanConditionParameter {
public:
	PlaceHolderScanCondition(const PlaceHolderScanCondition& inst);
	explicit PlaceHolderScanCondition(AbstractExpression* exp);
	virtual ~PlaceHolderScanCondition();

	virtual const UnicodeString* toStringCode() noexcept;
	virtual IValueProvider* clone() const noexcept;

	virtual AbstractCdbValue* evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata);

	virtual bool isAvailable(VirtualMachine* vm) const;

	void setAvailable(AbstractExpression* available);
	virtual bool hasPlaceholder() const noexcept {return true;}

private:
	void resetStr() noexcept;

private:
	AbstractExpression* exp;
	AbstractExpression* available;

	UnicodeString* str;
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_PARAMS_PLACEHOLDERSCANCONDITION_H_ */
