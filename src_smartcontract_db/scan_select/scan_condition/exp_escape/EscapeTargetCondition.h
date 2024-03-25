/*
 * EscapeTargetCondition.h
 *
 *  Created on: 2022/01/17
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_CONDITION_EXP_ESCAPE_ESCAPETARGETCONDITION_H_
#define SCAN_SELECT_SCAN_CONDITION_EXP_ESCAPE_ESCAPETARGETCONDITION_H_

namespace codablecash {

class AbstractScanTableTarget;
class IValueProvider;

class EscapeTargetCondition {
public:
	EscapeTargetCondition(const AbstractScanTableTarget* target);
	virtual ~EscapeTargetCondition();

	bool isEscape(IValueProvider* left);
private:
	const AbstractScanTableTarget* target;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_CONDITION_EXP_ESCAPE_ESCAPETARGETCONDITION_H_ */
