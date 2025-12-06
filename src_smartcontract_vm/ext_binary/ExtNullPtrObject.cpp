/*
 * ExtNullPtrObject.cpp
 *
 *  Created on: 2020/03/28
 *      Author: iizuka
 */

#include "ext_binary/ExtNullPtrObject.h"

#include "instance/VmInstanceTypesConst.h"

#include "base/UnicodeString.h"


namespace alinous {

ExtNullPtrObject::ExtNullPtrObject(const UnicodeString* name) : AbstractExtObject(name, VmInstanceTypesConst::INST_NULL){
}

ExtNullPtrObject::~ExtNullPtrObject() {
}

bool ExtNullPtrObject::isNull() const noexcept {
	return true;
}

AbstractExtObject* ExtNullPtrObject::copy() const noexcept {
	return new ExtNullPtrObject(this->name);
}

const UnicodeString* ExtNullPtrObject::toString() const noexcept {
	delete this->str;
	this->str = new UnicodeString(L"null");

	return this->str;
}

} /* namespace alinous */
