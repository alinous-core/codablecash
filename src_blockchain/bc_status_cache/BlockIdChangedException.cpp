/*
 * BlockIdChangedException.cpp
 *
 *  Created on: 2023/03/31
 *      Author: iizuka
 */

#include "bc_status_cache/BlockIdChangedException.h"

#include "base/UnicodeString.h"

namespace codablecash {

const wchar_t* BlockIdChangedException::defaultMessage = L"Header id has changed while pow result was made.";

BlockIdChangedException::BlockIdChangedException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
BlockIdChangedException::BlockIdChangedException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
BlockIdChangedException::BlockIdChangedException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
BlockIdChangedException::BlockIdChangedException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}

BlockIdChangedException::~BlockIdChangedException() {
}

} /* namespace codablecash */
