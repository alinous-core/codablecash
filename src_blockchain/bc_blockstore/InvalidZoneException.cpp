/*
 * InvalidZoneException.cpp
 *
 *  Created on: Dec 21, 2024
 *      Author: iizuka
 */

#include "bc_blockstore/InvalidZoneException.h"

#include "base/UnicodeString.h"

namespace codablecash {

const wchar_t* InvalidZoneException::defaultMessage = L"Format of Transaction binary is wrong. ";

InvalidZoneException::InvalidZoneException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
InvalidZoneException::InvalidZoneException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
InvalidZoneException::InvalidZoneException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
InvalidZoneException::InvalidZoneException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
InvalidZoneException::~InvalidZoneException() {
}


} /* namespace codablecash */
