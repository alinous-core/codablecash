/*
 * JsonNumericValue.cpp
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#include "json_object/JsonNumericValue.h"

namespace codablecash {

JsonNumericValue::JsonNumericValue(const JsonNumericValue &inst) {
	this->value = inst.value;
}

JsonNumericValue::JsonNumericValue() {
	this->value = 0;
}

JsonNumericValue::JsonNumericValue(int v) {
	this->value = v;
}

JsonNumericValue::~JsonNumericValue() {

}

AbstractJsonObject* JsonNumericValue::copy() const noexcept {
	return new JsonNumericValue(*this);
}

bool JsonNumericValue::equals(const AbstractJsonObject *other) const noexcept {
	const JsonNumericValue* v = dynamic_cast<const JsonNumericValue*>(other);

	return v != nullptr && this->value == v->value;
}

} /* namespace codablecash */
