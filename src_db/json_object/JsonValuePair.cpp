/*
 * JsonValuePair.cpp
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#include "json_object/JsonValuePair.h"
#include "json_object/AbstractJsonValue.h"
#include "json_object/AbstractJsonObject.h"


namespace codablecash {

JsonValuePair::JsonValuePair(const JsonValuePair& inst) {
	this->key = inst.key != nullptr ? dynamic_cast<AbstractJsonValue*>(inst.key->copy()) : nullptr;
	this->value = inst.value != nullptr ? dynamic_cast<AbstractJsonObject*>(inst.value->copy()) : nullptr;
}

JsonValuePair::JsonValuePair() {
	this->key = nullptr;
	this->value = nullptr;
}

JsonValuePair::~JsonValuePair() {
	delete this->key;
	delete this->value;
}

void JsonValuePair::setKey(AbstractJsonValue *key) noexcept {
	delete this->key;
	this->key = key;
}

void JsonValuePair::setValue(AbstractJsonObject *value) noexcept {
	delete this->value;
	this->value = value;
}

AbstractJsonObject* JsonValuePair::copy() const noexcept {
	return new JsonValuePair(*this);
}

bool JsonValuePair::equals(const AbstractJsonObject *other) const noexcept {
	bool bl = false;
	const JsonValuePair* v = dynamic_cast<const JsonValuePair*>(other);

	if(v != nullptr && this->key->equals(v->key) && this->value->equals(v->value)){
		bl = true;
	}

	return bl;
}

} /* namespace codablecash */
