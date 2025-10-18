/*
 * JsonBooleanValue.cpp
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#include "json_object/JsonBooleanValue.h"

namespace codablecash {

JsonBooleanValue::JsonBooleanValue(const JsonBooleanValue &inst) {
	this->value = inst.value;
}

JsonBooleanValue::JsonBooleanValue() {
	this->value = false;
}

JsonBooleanValue::~JsonBooleanValue() {

}

AbstractJsonObject* JsonBooleanValue::copy() const noexcept {
	return new JsonBooleanValue(*this);
}

bool JsonBooleanValue::equals(const AbstractJsonObject *other) const noexcept {
	const JsonBooleanValue* v = dynamic_cast<const JsonBooleanValue*>(other);

	return v != nullptr && this->value == v->value;
}

} /* namespace codablecash */
