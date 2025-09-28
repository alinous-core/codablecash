/*
 * JsonObject.cpp
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#include "json_object/JsonObject.h"
#include "json_object/JsonValuePair.h"


namespace codablecash {

JsonObject::JsonObject() {
	this->list = new ArrayList<JsonValuePair>();
}

JsonObject::~JsonObject() {
	this->list->deleteElements();
	delete this->list;
}

void JsonObject::add(JsonValuePair *pair) noexcept {
	this->list->addElement(pair);
}

const JsonValuePair* JsonObject::get(int pos) const noexcept {
	return this->list->get(pos);
}

} /* namespace codablecash */
