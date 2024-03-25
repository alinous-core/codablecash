/*
 * DataStructureException.cpp
 *
 *  Created on: 2023/11/09
 *      Author: iizuka
 */

#include "data_history_data/DataStructureException.h"

#include "base/UnicodeString.h"


namespace codablecash {

const wchar_t* DataStructureException::defaultMessage = L"Blockchain structure error. ";

DataStructureException::DataStructureException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
DataStructureException::DataStructureException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
DataStructureException::DataStructureException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
DataStructureException::DataStructureException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
DataStructureException::~DataStructureException() {
}

} /* namespace codablecash */
