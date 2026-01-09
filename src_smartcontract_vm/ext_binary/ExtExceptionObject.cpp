/*
 * ExtExceptionObject.cpp
 *
 *  Created on: 2020/04/16
 *      Author: iizuka
 */

#include "ext_binary/ExtExceptionObject.h"

#include "instance/VmInstanceTypesConst.h"

#include "base/UnicodeString.h"


namespace alinous {

ExtExceptionObject::ExtExceptionObject(const UnicodeString* name) : AbstractExtObject(name, VmInstanceTypesConst::INST_EXCEPTION) {
	this->element = nullptr;
	this->className = nullptr;
	this->message = nullptr;
}

ExtExceptionObject::~ExtExceptionObject() {
	this->element = nullptr;
	delete this->className;
	delete this->message;
}

AbstractExtObject* ExtExceptionObject::copy() const noexcept {
	ExtExceptionObject* newObj = new ExtExceptionObject(this->name);
	newObj->element = this->element;
	newObj->className = this->className == nullptr ? nullptr : new UnicodeString(this->className);
	newObj->message = this->message == nullptr ? nullptr : new UnicodeString(this->message);

	return newObj;
}

void ExtExceptionObject::setCodeElement(const CodeElement* element) noexcept {
	this->element = element;
}

const CodeElement* ExtExceptionObject::getCodeElement() const noexcept {
	return this->element;
}

void ExtExceptionObject::setClassName(const UnicodeString* className) noexcept {
	this->className = new UnicodeString(className);
}

const UnicodeString* ExtExceptionObject::getClassName() const noexcept {
	return this->className;
}

void ExtExceptionObject::setMessage(const UnicodeString *message) noexcept {
	this->message = new UnicodeString(message);
}

const UnicodeString* ExtExceptionObject::toString() const noexcept {
	delete this->str;
	this->str = new UnicodeString(L"@");

	this->str->append(this->className);

	return this->str;
}

const UnicodeString* ExtExceptionObject::getMessage() const noexcept {
	return this->message;
}

AbstractVmInstance* ExtExceptionObject::toVmInstance(VirtualMachine *vm) {
	// FIXME toVmInstance
}

} /* namespace alinous */
