/*
 * RetryableNode2NodeAccessException.cpp
 *
 *  Created on: 2023/11/19
 *      Author: iizuka
 */

#include "bc_p2p_cmd_node_access/RetryableNode2NodeAccessException.h"

#include "base/UnicodeString.h"

namespace codablecash {

const wchar_t* RetryableNode2NodeAccessException::defaultMessage = L"All of P2P nodes has error. ";

RetryableNode2NodeAccessException::RetryableNode2NodeAccessException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
RetryableNode2NodeAccessException::RetryableNode2NodeAccessException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
RetryableNode2NodeAccessException::RetryableNode2NodeAccessException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
RetryableNode2NodeAccessException::RetryableNode2NodeAccessException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
RetryableNode2NodeAccessException::~RetryableNode2NodeAccessException() {
}

} /* namespace codablecash */
