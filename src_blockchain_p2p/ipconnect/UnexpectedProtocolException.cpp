/*
 * UnexpectedProtocolException.cpp
 *
 *  Created on: 2023/09/09
 *      Author: iizuka
 */

#include "ipconnect/UnexpectedProtocolException.h"

#include "base/UnicodeString.h"

namespace codablecash {

const wchar_t* UnexpectedProtocolException::defaultMessage = L"It supports only IPv4 and Ipv6.";

UnexpectedProtocolException::UnexpectedProtocolException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
UnexpectedProtocolException::UnexpectedProtocolException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
UnexpectedProtocolException::UnexpectedProtocolException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
UnexpectedProtocolException::UnexpectedProtocolException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
UnexpectedProtocolException::~UnexpectedProtocolException() {
}

} /* namespace codablecash */
