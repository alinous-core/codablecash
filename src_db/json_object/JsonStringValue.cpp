/*
 * JsonStringValue.cpp
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#include "json_object/JsonStringValue.h"

#include "base/UnicodeString.h"


namespace codablecash {

JsonStringValue::JsonStringValue(const JsonStringValue &inst) {
	this->value = new UnicodeString(inst.value);
}

JsonStringValue::JsonStringValue() {
	this->value = nullptr;
}

JsonStringValue::JsonStringValue(const wchar_t *str) {
	this->value = new UnicodeString(str);
}

JsonStringValue::~JsonStringValue() {
	delete this->value;
}

void JsonStringValue::setValue(const UnicodeString *value) noexcept {
	delete this->value;
	this->value = new UnicodeString(value);
}

AbstractJsonObject* JsonStringValue::copy() const noexcept {
	return new JsonStringValue(*this);
}

bool JsonStringValue::equals(const AbstractJsonObject *other) const noexcept {
	const JsonStringValue* otherString = dynamic_cast<const JsonStringValue*>(other);

	const UnicodeString* str = nullptr;
	if(otherString != nullptr) {
		str = otherString->getValue();
	}

	return otherString != nullptr && this->value->equals(str);
}

} /* namespace codablecash */
