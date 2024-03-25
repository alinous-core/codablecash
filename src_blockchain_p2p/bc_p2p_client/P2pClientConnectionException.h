/*
 * P2pClientConnectionException.h
 *
 *  Created on: 2023/09/09
 *      Author: iizuka
 */

#ifndef BC_P2P_CLIENT_P2PCLIENTCONNECTIONEXCEPTION_H_
#define BC_P2P_CLIENT_P2PCLIENTCONNECTIONEXCEPTION_H_

#include "base/Exception.h"

using namespace alinous;

namespace codablecash {

class P2pClientConnectionException : public Exception {
public:
	P2pClientConnectionException(const char* srcfile, int srcline) noexcept;
	P2pClientConnectionException(Exception* cause, const char* srcfile, int srcline) noexcept;
	P2pClientConnectionException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	P2pClientConnectionException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~P2pClientConnectionException();
	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* BC_P2P_CLIENT_P2PCLIENTCONNECTIONEXCEPTION_H_ */
