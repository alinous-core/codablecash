/*
 * StringLiteralColumnParam.cpp
 *
 *  Created on: 2020/08/20
 *      Author: iizuka
 */

#include "scan_select/scan_columns/scan_columns_params/StringLiteralColumnParam.h"

#include "base/UnicodeString.h"

namespace codablecash {

StringLiteralColumnParam::StringLiteralColumnParam(const UnicodeString* str) {
	this->value = new UnicodeString(str);
}

StringLiteralColumnParam::~StringLiteralColumnParam() {
	delete this->value;
}

const UnicodeString* StringLiteralColumnParam::toStringCode() noexcept {
	if(this->str == nullptr){
		this->str = new UnicodeString(L"'");
		this->str->append(this->value);
		this->str->append(L"'");
	}

	return this->str;
}

} /* namespace codablecash */
