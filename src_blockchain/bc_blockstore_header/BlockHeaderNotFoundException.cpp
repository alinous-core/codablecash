/*
 * BlockHeaderNotFoundException.cpp
 *
 *  Created on: 2023/12/18
 *      Author: iizuka
 */

#include "bc_blockstore_header/BlockHeaderNotFoundException.h"

#include "base/UnicodeString.h"

using alinous::UnicodeString;

namespace codablecash {

const wchar_t* BlockHeaderNotFoundException::defaultMessage = L"Blockheader does not exists. ";

BlockHeaderNotFoundException::BlockHeaderNotFoundException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
BlockHeaderNotFoundException::BlockHeaderNotFoundException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
BlockHeaderNotFoundException::BlockHeaderNotFoundException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
BlockHeaderNotFoundException::BlockHeaderNotFoundException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
BlockHeaderNotFoundException::~BlockHeaderNotFoundException() {
}

} /* namespace codablecash */
