/*
 * ModuleConfigException.cpp
 *
 *  Created on: Oct 4, 2025
 *      Author: iizuka
 */

#include "modular_project/ModularConfigException.h"

#include "base/UnicodeString.h"


namespace codablecash {

const wchar_t* ModularConfigException::defaultMessage = L"Modular Smartcontract Porject configuration error. ";

ModularConfigException::ModularConfigException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
ModularConfigException::ModularConfigException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
ModularConfigException::ModularConfigException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
ModularConfigException::ModularConfigException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
ModularConfigException::~ModularConfigException() {
}

} /* namespace codablecash */
