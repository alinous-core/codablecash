/*
 * P2pHandshakeAuthenticationException.h
 *
 *  Created on: 2023/04/06
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_P2PHANDSHAKEAUTHENTICATIONEXCEPTION_H_
#define BC_P2P_CMD_P2PHANDSHAKEAUTHENTICATIONEXCEPTION_H_

#include "base/Exception.h"

namespace codablecash {

class P2pHandshakeAuthenticationException : public alinous::Exception {
public:
	P2pHandshakeAuthenticationException(const char* srcfile, int srcline) noexcept;
	P2pHandshakeAuthenticationException(Exception* cause, const char* srcfile, int srcline) noexcept;
	P2pHandshakeAuthenticationException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	P2pHandshakeAuthenticationException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~P2pHandshakeAuthenticationException();

	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_P2PHANDSHAKEAUTHENTICATIONEXCEPTION_H_ */
