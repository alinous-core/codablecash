/*
 * PubsubNetworkException.cpp
 *
 *  Created on: Sep 8, 2024
 *      Author: iizuka
 */

#include "pubsub/PubsubNetworkException.h"

#include "base/UnicodeString.h"

using alinous::UnicodeString;

namespace codablecash {

const wchar_t* PubsubNetworkException::defaultMessage = L"Pubsub Network Error.";

PubsubNetworkException::PubsubNetworkException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
PubsubNetworkException::PubsubNetworkException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
PubsubNetworkException::PubsubNetworkException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
PubsubNetworkException::PubsubNetworkException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}

PubsubNetworkException::~PubsubNetworkException() {
}
}
