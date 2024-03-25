/*
 * InvaridTransactionException.cpp
 *
 *  Created on: 2023/10/11
 *      Author: iizuka
 */

#include "base/UnicodeString.h"
#include "bc_p2p_cmd_node/InvalidTransactionException.h"

namespace codablecash {

const wchar_t* InvalidTransactionException::defaultMessage = L"Blockchain Transaction error. ";

InvalidTransactionException::InvalidTransactionException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
InvalidTransactionException::InvalidTransactionException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
InvalidTransactionException::InvalidTransactionException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
InvalidTransactionException::InvalidTransactionException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
InvalidTransactionException::~InvalidTransactionException() {
}

} /* namespace codablecash */
