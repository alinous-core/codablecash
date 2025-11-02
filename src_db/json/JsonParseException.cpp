/*
 * JsonParseException.cpp
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#include "json/JsonParseException.h"

#include "base/UnicodeString.h"


namespace codablecash {

const wchar_t* JsonParseException::defaultMessage = L"Json format error. ";

JsonParseException::JsonParseException(const char *srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}

JsonParseException::JsonParseException(Exception *cause, const char *srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}

JsonParseException::JsonParseException(const wchar_t *message, const char *srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}

JsonParseException::JsonParseException(const wchar_t *message, Exception *cause, const char *srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}

JsonParseException::~JsonParseException() {
}
} /* namespace codablecash */
