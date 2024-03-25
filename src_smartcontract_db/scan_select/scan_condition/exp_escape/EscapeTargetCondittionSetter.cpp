/*
 * EscapeTargetCondittionSetter.cpp
 *
 *  Created on: 2022/01/17
 *      Author: iizuka
 */

#include "scan_select/scan_condition/exp_escape/EscapeTargetCondittionSetter.h"

#include "vm/VirtualMachine.h"

#include "scan_select/scan_condition/exp_escape/EscapeTargetCondition.h"
namespace codablecash {

EscapeTargetCondittionSetter::EscapeTargetCondittionSetter(VirtualMachine* vm, const AbstractScanTableTarget* target) {
	this->vm = vm;
	EscapeTargetCondition* cond = new EscapeTargetCondition(target);
	this->vm->setEscapeTargetCondition(cond);
}

EscapeTargetCondittionSetter::~EscapeTargetCondittionSetter() {
	this->vm->setEscapeTargetCondition(nullptr); // delete
}

} /* namespace codablecash */
