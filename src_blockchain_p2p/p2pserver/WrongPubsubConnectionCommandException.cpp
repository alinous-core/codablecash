/*
 * WrongPubsubConnectionCommandException.cpp
 *
 *  Created on: 2023/09/11
 *      Author: iizuka
 */

#include "p2pserver/WrongPubsubConnectionCommandException.h"

#include "base/UnicodeString.h"

namespace codablecash {

const wchar_t* WrongPubsubConnectionCommandException::defaultMessage = L"Wrong pubsub connection command. ";

WrongPubsubConnectionCommandException::WrongPubsubConnectionCommandException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
WrongPubsubConnectionCommandException::WrongPubsubConnectionCommandException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
WrongPubsubConnectionCommandException::WrongPubsubConnectionCommandException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
WrongPubsubConnectionCommandException::WrongPubsubConnectionCommandException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
WrongPubsubConnectionCommandException::~WrongPubsubConnectionCommandException() {
}

} /* namespace codablecash */
