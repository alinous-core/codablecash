/*
 * EscapeTargetCondittionSetter.h
 *
 *  Created on: 2022/01/17
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_CONDITION_EXP_ESCAPE_ESCAPETARGETCONDITTIONSETTER_H_
#define SCAN_SELECT_SCAN_CONDITION_EXP_ESCAPE_ESCAPETARGETCONDITTIONSETTER_H_

namespace alinous {
class VirtualMachine;
}
using namespace alinous;

namespace codablecash {

class AbstractScanTableTarget;

class EscapeTargetCondittionSetter {
public:
	EscapeTargetCondittionSetter(VirtualMachine* vm, const AbstractScanTableTarget* target);
	virtual ~EscapeTargetCondittionSetter();

private:
	VirtualMachine* vm;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_CONDITION_EXP_ESCAPE_ESCAPETARGETCONDITTIONSETTER_H_ */
