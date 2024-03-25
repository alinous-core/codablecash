/*
 * P2pHandshakeAuthenticationException.cpp
 *
 *  Created on: 2023/04/06
 *      Author: iizuka
 */

#include "bc_p2p_cmd/P2pHandshakeAuthenticationException.h"

#include "base/UnicodeString.h"

using namespace alinous;

namespace codablecash {

const wchar_t* P2pHandshakeAuthenticationException::defaultMessage = L"Signature of the P2p command is wrong.";

P2pHandshakeAuthenticationException::P2pHandshakeAuthenticationException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
P2pHandshakeAuthenticationException::P2pHandshakeAuthenticationException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
P2pHandshakeAuthenticationException::P2pHandshakeAuthenticationException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
P2pHandshakeAuthenticationException::P2pHandshakeAuthenticationException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}

P2pHandshakeAuthenticationException::~P2pHandshakeAuthenticationException() {
}

} /* namespace codablecash */
