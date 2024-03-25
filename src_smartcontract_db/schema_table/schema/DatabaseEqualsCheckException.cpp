/*
 * DatabaseEqualsCheckException.cpp
 *
 *  Created on: 2023/07/29
 *      Author: iizuka
 */

#include "schema_table/schema/DatabaseEqualsCheckException.h"

#include "base/UnicodeString.h"

namespace codablecash {

const wchar_t* DatabaseEqualsCheckException::defaultMessage = L"Database equals check error. ";

DatabaseEqualsCheckException::DatabaseEqualsCheckException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
DatabaseEqualsCheckException::DatabaseEqualsCheckException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
DatabaseEqualsCheckException::DatabaseEqualsCheckException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
DatabaseEqualsCheckException::DatabaseEqualsCheckException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
DatabaseEqualsCheckException::~DatabaseEqualsCheckException() {
}

} /* namespace codablecash */
