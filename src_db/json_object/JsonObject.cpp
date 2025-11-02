/*
 * JsonObject.cpp
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#include "json_object/JsonObject.h"
#include "json_object/JsonValuePair.h"
#include "json_object/AbstractJsonValue.h"
#include "json_object/JsonStringValue.h"

#include "base/UnicodeString.h"


using namespace alinous;

namespace codablecash {

JsonObject::JsonObject(const JsonObject &inst) {
	this->list = new ArrayList<JsonValuePair>();

	int maxLoop = inst.list->size();
	for(int i = 0; i != maxLoop; ++i){
		JsonValuePair* pair = inst.list->get(i);

		this->list->addElement(dynamic_cast<JsonValuePair*>(pair->copy()));
	}
}

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

const JsonValuePair* JsonObject::get(const wchar_t *name) const noexcept {
	UnicodeString str(name);
	JsonValuePair* ret = nullptr;

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		JsonValuePair* pair = this->list->get(i);

		AbstractJsonValue* key = pair->getKey();
		JsonStringValue* stringValue = dynamic_cast<JsonStringValue*>(key);
		if(stringValue != nullptr){
			const UnicodeString* n = stringValue->getValue();

			if(str.equals(n)){
				ret = pair;
			}
		}
	}

	return ret;
}

AbstractJsonObject* JsonObject::copy() const noexcept {
	return new JsonObject(*this);
}

bool JsonObject::equals(const AbstractJsonObject *other) const noexcept {
	bool bl = false;
	const JsonObject* v = dynamic_cast<const JsonObject*>(other);

	if(v != nullptr && size() == v->size()){
		int maxLoop = this->list->size();
		for(int i = 0; i != maxLoop; ++i){
			JsonValuePair* pair = this->list->get(i);
			JsonValuePair* vpair = v->list->get(i);

			if(!pair->equals(vpair)){
				return false;
			}
		}

		bl = true;
	}

	return bl;
}

int JsonObject::size() const noexcept {
	return this->list->size();
}

} /* namespace codablecash */
