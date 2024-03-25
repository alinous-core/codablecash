/*
 * EscapeTargetCondition.cpp
 *
 *  Created on: 2022/01/17
 *      Author: iizuka
 */

#include "scan_select/scan_condition/exp_escape/EscapeTargetCondition.h"

#include "scan_select/scan_condition/params/ColumnIdentifierScanParam.h"

#include "scan_select/scan_table/AbstractScanTableTarget.h"

namespace codablecash {

EscapeTargetCondition::EscapeTargetCondition(const AbstractScanTableTarget* target) {
	this->target = target;
}

EscapeTargetCondition::~EscapeTargetCondition() {
	this->target = nullptr;
}

bool EscapeTargetCondition::isEscape(IValueProvider* vp) {
	bool ret = false;

	ColumnIdentifierScanParam* param = dynamic_cast<ColumnIdentifierScanParam*>(vp);
	if(param != nullptr){
		const AbstractScanTableTarget* paramTarget = param->getSourceTarget();

		if(this->target->hasTarget(paramTarget)){
			ret = true;
		}
	}

	return ret;
}


} /* namespace codablecash */
