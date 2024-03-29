/*
 * ExtStringClass.cpp
 *
 *  Created on: 2020/03/18
 *      Author: iizuka
 */

#include "ext_binary/ExtStringClass.h"

#include "instance/VmInstanceTypesConst.h"

#include "base/UnicodeString.h"

namespace alinous {

ExtStringClass::ExtStringClass(const UnicodeString* name, const UnicodeString* value)
					: AbstractExtObject(name, VmInstanceTypesConst::INST_STRING) {
	this->value = new UnicodeString(value);
}

ExtStringClass::ExtStringClass(const UnicodeString* name) : AbstractExtObject(name, VmInstanceTypesConst::INST_STRING) {
	this->value = nullptr;
}

ExtStringClass::~ExtStringClass() {
	delete this->value;
}

const UnicodeString* ExtStringClass::getValue() const noexcept {
	return this->value;
}

AbstractExtObject* ExtStringClass::copy() const noexcept {
	return new ExtStringClass(getName(), getValue());
}

const UnicodeString* ExtStringClass::toString() const noexcept {
	delete this->str;
	this->str = new UnicodeString(L"'");

	this->str->append(this->value);

	this->str->append(L"'");
	return this->str;

}


} /* namespace alinous */
