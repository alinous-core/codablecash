/*
 * HandshakeConnectionException.cpp
 *
 *  Created on: 2023/01/10
 *      Author: iizuka
 */

#include "pubsub/HandshakeConnectionException.h"

#include "base/UnicodeString.h"

using namespace alinous;

namespace codablecash {

const wchar_t* HandshakeConnectionException::defaultMessage = L"Network Connection Error.";

HandshakeConnectionException::HandshakeConnectionException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
HandshakeConnectionException::HandshakeConnectionException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
HandshakeConnectionException::HandshakeConnectionException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
HandshakeConnectionException::HandshakeConnectionException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}

HandshakeConnectionException::~HandshakeConnectionException() {
}

} /* namespace codablecash */
