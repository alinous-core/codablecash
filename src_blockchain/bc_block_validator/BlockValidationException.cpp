/*
 * BlockValidatorException.cpp
 *
 *  Created on: 2023/11/27
 *      Author: iizuka
 */

#include "base/UnicodeString.h"
#include "bc_block_validator/BlockValidationException.h"

using alinous::UnicodeString;

namespace codablecash {

const wchar_t* BlockValidationException::defaultMessage = L"Blockchain Transaction error. ";

BlockValidationException::BlockValidationException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
BlockValidationException::BlockValidationException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
BlockValidationException::BlockValidationException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
BlockValidationException::BlockValidationException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
BlockValidationException::~BlockValidationException() {
}

} /* namespace codablecash */
