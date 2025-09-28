/*
 * JsonStringValue.cpp
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#include "json_object/JsonStringValue.h"

#include "base/UnicodeString.h"


namespace codablecash {

JsonStringValue::JsonStringValue() {
	this->value = nullptr;
}

JsonStringValue::~JsonStringValue() {
	delete this->value;
}

void JsonStringValue::setValue(const UnicodeString *value) noexcept {
	delete this->value;
	this->value = new UnicodeString(value);
}

} /* namespace codablecash */
