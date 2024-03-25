/*
 * UnexpectedProtocolException.h
 *
 *  Created on: 2023/09/09
 *      Author: iizuka
 */

#ifndef IPCONNECT_UNEXPECTEDPROTOCOLEXCEPTION_H_
#define IPCONNECT_UNEXPECTEDPROTOCOLEXCEPTION_H_

#include "base/Exception.h"

using namespace alinous;

namespace codablecash {

class UnexpectedProtocolException : public Exception {
public:
	UnexpectedProtocolException(const char* srcfile, int srcline) noexcept;
	UnexpectedProtocolException(Exception* cause, const char* srcfile, int srcline) noexcept;
	UnexpectedProtocolException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	UnexpectedProtocolException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~UnexpectedProtocolException();
	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* IPCONNECT_UNEXPECTEDPROTOCOLEXCEPTION_H_ */
