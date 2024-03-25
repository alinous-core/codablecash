/*
 * FeeShortageException.h
 *
 *  Created on: 2023/02/25
 *      Author: iizuka
 */

#ifndef BC_TRX_BALANCE_FEESHORTAGEEXCEPTION_H_
#define BC_TRX_BALANCE_FEESHORTAGEEXCEPTION_H_

#include "base/Exception.h"

namespace codablecash {

class FeeShortageException : public alinous::Exception {
public:
	FeeShortageException(const char* srcfile, int srcline) noexcept;
	FeeShortageException(Exception* cause, const char* srcfile, int srcline) noexcept;
	FeeShortageException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	FeeShortageException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~FeeShortageException();

	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* BC_TRX_BALANCE_FEESHORTAGEEXCEPTION_H_ */
