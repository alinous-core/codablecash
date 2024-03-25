/*
 * AbstractSQLFunctionBody.cpp
 *
 *  Created on: 2021/10/26
 *      Author: iizuka
 */

#include "scan_select/scan_condition/func/AbstractSQLFunctionBody.h"

#include "base/UnicodeString.h"

namespace codablecash {

AbstractSQLFunctionBody::AbstractSQLFunctionBody(UnicodeString* name) {
	this->name = name;
}

AbstractSQLFunctionBody::~AbstractSQLFunctionBody() {
	delete this->name;
}

bool AbstractSQLFunctionBody::isAggregate() const noexcept {
	return false;
}

} /* namespace codablecash */
