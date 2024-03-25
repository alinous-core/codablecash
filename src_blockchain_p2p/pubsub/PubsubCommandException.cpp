/*
 * PubsubCommandException.cpp
 *
 *  Created on: 2023/01/13
 *      Author: iizuka
 */

#include "pubsub/PubsubCommandException.h"

#include "base/UnicodeString.h"

using namespace alinous;

namespace codablecash {

const wchar_t* PubsubCommandException::defaultMessage = L"Pubsub Command Error.";

PubsubCommandException::PubsubCommandException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
PubsubCommandException::PubsubCommandException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
PubsubCommandException::PubsubCommandException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
PubsubCommandException::PubsubCommandException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}

PubsubCommandException::~PubsubCommandException() {
}

} /* namespace codablecash */
