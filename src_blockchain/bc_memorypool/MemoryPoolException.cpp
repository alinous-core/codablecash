/*
 * MemoryPoolException.cpp
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#include "bc_memorypool/MemoryPoolException.h"

using alinous::UnicodeString;

#include "base/UnicodeString.h"

namespace codablecash {

const wchar_t* MemoryPoolException::defaultMessage = L"Memory pool exception. ";

MemoryPoolException::MemoryPoolException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
MemoryPoolException::MemoryPoolException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
MemoryPoolException::MemoryPoolException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
MemoryPoolException::MemoryPoolException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}

MemoryPoolException::~MemoryPoolException() {
}

} /* namespace codablecash */
