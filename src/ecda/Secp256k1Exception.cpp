/*
 * Secp256k1Exception.cpp
 *
 *  Created on: 2023/02/03
 *      Author: iizuka
 */

#include "ecda/Secp256k1Exception.h"

#include "base/UnicodeString.h"

using namespace alinous;

namespace codablecash {

const wchar_t* Secp256k1Exception::defaultMessage = L"Secp256k1 value error.";

Secp256k1Exception::Secp256k1Exception(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
Secp256k1Exception::Secp256k1Exception(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
Secp256k1Exception::Secp256k1Exception(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
Secp256k1Exception::Secp256k1Exception(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}

Secp256k1Exception::~Secp256k1Exception() {
}

} /* namespace codablecash */
