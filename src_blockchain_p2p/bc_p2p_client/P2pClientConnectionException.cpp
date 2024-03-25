/*
 * P2pClientConnectionException.cpp
 *
 *  Created on: 2023/09/09
 *      Author: iizuka
 */

#include "bc_p2p_client/P2pClientConnectionException.h"

#include "base/UnicodeString.h"

namespace codablecash {

const wchar_t* P2pClientConnectionException::defaultMessage = L"Failed in connecting. ";

P2pClientConnectionException::P2pClientConnectionException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
P2pClientConnectionException::P2pClientConnectionException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
P2pClientConnectionException::P2pClientConnectionException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
P2pClientConnectionException::P2pClientConnectionException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
P2pClientConnectionException::~P2pClientConnectionException() {
}

} /* namespace codablecash */
