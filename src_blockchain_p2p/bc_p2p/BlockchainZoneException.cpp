/*
 * ZoneNotExistException.cpp
 *
 *  Created on: 2023/08/11
 *      Author: iizuka
 */

#include "bc_p2p/BlockchainZoneException.h"

#include "base/UnicodeString.h"

namespace codablecash {

const wchar_t* BlockchainZoneException::defaultMessage = L"The Blockchain Zone has error. ";

BlockchainZoneException::BlockchainZoneException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
BlockchainZoneException::BlockchainZoneException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
BlockchainZoneException::BlockchainZoneException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
BlockchainZoneException::BlockchainZoneException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
BlockchainZoneException::~BlockchainZoneException() {
}


} /* namespace codablecash */
