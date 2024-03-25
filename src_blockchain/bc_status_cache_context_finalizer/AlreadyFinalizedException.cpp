/*
 * .cpp
 *
 *  Created on: 2023/05/29
 *      Author: iizuka
 */

#include "bc_status_cache_context_finalizer/AlreadyFinalizedException.h"

using alinous::UnicodeString;

#include "base/UnicodeString.h"

namespace codablecash {

const wchar_t* AlreadyFinalizedException::defaultMessage = L"Secp256k1 value error.";

AlreadyFinalizedException::AlreadyFinalizedException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
AlreadyFinalizedException::AlreadyFinalizedException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
AlreadyFinalizedException::AlreadyFinalizedException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
AlreadyFinalizedException::AlreadyFinalizedException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}

AlreadyFinalizedException::~AlreadyFinalizedException() {
}
} /* namespace codablecash */
