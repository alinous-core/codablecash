/*
 * BalanceShortageException.h
 *
 *  Created on: 2023/02/26
 *      Author: iizuka
 */

#ifndef BC_TRX_BALANCE_BALANCESHORTAGEEXCEPTION_H_
#define BC_TRX_BALANCE_BALANCESHORTAGEEXCEPTION_H_

#include "base/Exception.h"

namespace codablecash {

class BalanceShortageException : public alinous::Exception {
public:
	BalanceShortageException(const char* srcfile, int srcline) noexcept;
	BalanceShortageException(Exception* cause, const char* srcfile, int srcline) noexcept;
	BalanceShortageException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	BalanceShortageException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~BalanceShortageException();

	static const wchar_t* defaultMessage;
};


} /* namespace codablecash */

#endif /* BC_TRX_BALANCE_BALANCESHORTAGEEXCEPTION_H_ */
