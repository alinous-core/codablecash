/*
 * AbstractColumnsIndexWrapper.cpp
 *
 *  Created on: 2020/11/26
 *      Author: iizuka
 */

#include "base/UnicodeString.h"
#include "scan_select/scan_planner/scanner/index_resolv/AbstractColumnsIndexAccess.h"

namespace codablecash {

AbstractColumnsIndexAccess::AbstractColumnsIndexAccess(
		const AbstractColumnsIndexAccess& inst) {
	this->target = inst.target;
	this->index = inst.index;
	this->str = nullptr;
}


AbstractColumnsIndexAccess::AbstractColumnsIndexAccess() {
	this->target = nullptr;
	this->index = nullptr;
	this->str = nullptr;
}

AbstractColumnsIndexAccess::~AbstractColumnsIndexAccess() {
	this->target = nullptr;
	this->index = nullptr;

	resetStr();
}

void AbstractColumnsIndexAccess::resetStr() noexcept {
	delete this->str;
	this->str = nullptr;
}

} /* namespace codablecash */
