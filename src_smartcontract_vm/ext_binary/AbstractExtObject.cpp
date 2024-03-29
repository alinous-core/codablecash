/*
 * AbstractExtObject.cpp
 *
 *  Created on: 2019/12/18
 *      Author: iizuka
 */

#include "ext_binary/AbstractExtObject.h"

#include "base/UnicodeString.h"

#include "instance/VmInstanceTypesConst.h"
namespace alinous {

AbstractExtObject::AbstractExtObject(const UnicodeString* name, uint8_t type) {
	this->name = new UnicodeString(name);
	this->type = type;

	this->str = nullptr;
}

AbstractExtObject::~AbstractExtObject() {
	delete this->name;
	delete this->str;
}

const UnicodeString* AbstractExtObject::getName() const noexcept {
	return this->name;
}

uint8_t AbstractExtObject::getType() const noexcept {
	return this->type;
}

void AbstractExtObject::setName(const UnicodeString* name) noexcept {
	delete this->name;
	this->name = new UnicodeString(name);
}

bool AbstractExtObject::isNull() const noexcept {
	return false;
}

} /* namespace alinous */
