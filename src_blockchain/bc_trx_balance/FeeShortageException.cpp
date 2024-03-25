/*
 * FeeShortageException.cpp
 *
 *  Created on: 2023/02/25
 *      Author: iizuka
 */

#include "bc_trx_balance/FeeShortageException.h"

#include "base/UnicodeString.h"

using namespace alinous;

namespace codablecash {

const wchar_t* FeeShortageException::defaultMessage = L"Account Does not have enough balance.";

FeeShortageException::FeeShortageException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
FeeShortageException::FeeShortageException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
FeeShortageException::FeeShortageException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
FeeShortageException::FeeShortageException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}

FeeShortageException::~FeeShortageException() {
}

} /* namespace codablecash */
