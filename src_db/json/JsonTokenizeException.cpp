/*
 * JsonTokenizeException.cpp
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#include "json/JsonTokenizeException.h"

#include "base/UnicodeString.h"

namespace codablecash {

const wchar_t* JsonTokenizeException::defaultMessage = L"Token format error. ";

JsonTokenizeException::JsonTokenizeException(const char *srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}

JsonTokenizeException::JsonTokenizeException(Exception *cause, const char *srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}

JsonTokenizeException::JsonTokenizeException(const wchar_t *message, const char *srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}

JsonTokenizeException::JsonTokenizeException(const wchar_t *message, Exception *cause, const char *srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}

JsonTokenizeException::~JsonTokenizeException() {
}

} /* namespace codablecash */
