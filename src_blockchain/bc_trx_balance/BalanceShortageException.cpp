/*
 * BalanceShortageException.cpp
 *
 *  Created on: 2023/02/26
 *      Author: iizuka
 */

#include "bc_trx_balance/BalanceShortageException.h"

#include "base/UnicodeString.h"

using namespace alinous;

namespace codablecash {

const wchar_t* BalanceShortageException::defaultMessage = L"Account Does not have enough balance.";

BalanceShortageException::BalanceShortageException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
BalanceShortageException::BalanceShortageException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
BalanceShortageException::BalanceShortageException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
BalanceShortageException::BalanceShortageException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}

BalanceShortageException::~BalanceShortageException() {
}


} /* namespace codablecash */
