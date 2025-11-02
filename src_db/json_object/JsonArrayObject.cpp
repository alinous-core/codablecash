/*
 * JsonArrayObject.cpp
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#include "json_object/JsonArrayObject.h"
#include "json_object/AbstractJsonObject.h"


namespace codablecash {

JsonArrayObject::JsonArrayObject(const JsonArrayObject &inst) {
	this->list = new ArrayList<AbstractJsonObject>();

	int maxLoop = inst.list->size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractJsonObject* obj = inst.list->get(i);

		this->list->addElement(obj->copy());
	}
}

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

int JsonArrayObject::size() const noexcept {
	return this->list->size();
}

AbstractJsonObject* JsonArrayObject::get(int pos) const noexcept {
	return this->list->get(pos);
}

AbstractJsonObject* JsonArrayObject::copy() const noexcept {
	return new JsonArrayObject(*this);
}

bool JsonArrayObject::equals(const AbstractJsonObject *other) const noexcept {
	bool bl = false;
	const JsonArrayObject* v = dynamic_cast<const JsonArrayObject*>(other);

	if(v != nullptr && size() == v->size()){
		int maxLoop = size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractJsonObject* _this = this->list->get(i);
			AbstractJsonObject* _v = v->get(i);

			if(!_this->equals(_v)){
				return false;
			}
		}

		bl = true;
	}

	return bl;
}

} /* namespace codablecash */
