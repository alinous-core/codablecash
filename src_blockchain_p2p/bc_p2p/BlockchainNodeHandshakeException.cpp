/*
 * BlockchainNodeHandshakeException.cpp
 *
 *  Created on: 2023/11/05
 *      Author: iizuka
 */

#include "bc_p2p/BlockchainNodeHandshakeException.h"

#include "base/UnicodeString.h"

namespace codablecash {

const wchar_t* BlockchainNodeHandshakeException::defaultMessage = L"The p2p handshake has error. ";

BlockchainNodeHandshakeException::BlockchainNodeHandshakeException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
BlockchainNodeHandshakeException::BlockchainNodeHandshakeException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
BlockchainNodeHandshakeException::BlockchainNodeHandshakeException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
BlockchainNodeHandshakeException::BlockchainNodeHandshakeException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
BlockchainNodeHandshakeException::~BlockchainNodeHandshakeException() {
}

} /* namespace codablecash */
