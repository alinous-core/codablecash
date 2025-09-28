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

} /* namespace codablecash */
