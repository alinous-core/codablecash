/*
 * BlockchainStoreException.cpp
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#include "bc_blockstore/BlockchainStoreException.h"

#include "base/UnicodeString.h"

using namespace alinous;

namespace codablecash {

const wchar_t* BlockchainStoreException::defaultMessage = L"Blockchain Storage Error.";

BlockchainStoreException::BlockchainStoreException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
BlockchainStoreException::BlockchainStoreException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
BlockchainStoreException::BlockchainStoreException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
BlockchainStoreException::BlockchainStoreException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}

BlockchainStoreException::~BlockchainStoreException() {
}

} /* namespace codablecash */
