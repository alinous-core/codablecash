/*
 * IndexValidationException.cpp
 *
 *  Created on: 2023/07/27
 *      Author: iizuka
 */

#include "schema_table/table_store/IndexValidationException.h"

#include "base/UnicodeString.h"

namespace codablecash {


const wchar_t* IndexValidationException::defaultMessage = L"Index test error. ";

IndexValidationException::IndexValidationException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
IndexValidationException::IndexValidationException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
IndexValidationException::IndexValidationException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
IndexValidationException::IndexValidationException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}

IndexValidationException::~IndexValidationException() {
}


} /* namespace codablecash */
