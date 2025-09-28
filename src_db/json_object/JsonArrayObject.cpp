/*
 * JsonArrayObject.cpp
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#include "json_object/JsonArrayObject.h"
#include "json_object/AbstractJsonObject.h"


namespace codablecash {

JsonArrayObject::JsonArrayObject() {
	this->list = new ArrayList<AbstractJsonObject>();
}

JsonArrayObject::~JsonArrayObject() {
	this->list->deleteElements();
	delete this->list;
}

void JsonArrayObject::add(AbstractJsonObject *jsonObject) noexcept {
	this->list->addElement(jsonObject);
}

} /* namespace codablecash */
