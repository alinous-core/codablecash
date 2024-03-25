/*
 * SignatureVerifivcationErrorException.cpp
 *
 *  Created on: 2023/10/02
 *      Author: iizuka
 */

#include "bc_p2p_cmd/SignatureVerifivcationErrorException.h"

#include "base/UnicodeString.h"

namespace codablecash {

const wchar_t* SignatureVerifivcationErrorException::defaultMessage = L"Signature of the command is wrong. ";

SignatureVerifivcationErrorException::SignatureVerifivcationErrorException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
SignatureVerifivcationErrorException::SignatureVerifivcationErrorException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
SignatureVerifivcationErrorException::SignatureVerifivcationErrorException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
SignatureVerifivcationErrorException::SignatureVerifivcationErrorException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
SignatureVerifivcationErrorException::~SignatureVerifivcationErrorException() {
}
} /* namespace codablecash */
