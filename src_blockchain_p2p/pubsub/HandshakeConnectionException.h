/*
 * HandshakeConnectionException.h
 *
 *  Created on: 2023/01/10
 *      Author: iizuka
 */

#ifndef PUBSUB_HANDSHAKECONNECTIONEXCEPTION_H_
#define PUBSUB_HANDSHAKECONNECTIONEXCEPTION_H_

#include "base/Exception.h"

namespace codablecash {

class HandshakeConnectionException: public alinous::Exception {
public:
	HandshakeConnectionException(const char* srcfile, int srcline) noexcept;
	HandshakeConnectionException(Exception* cause, const char* srcfile, int srcline) noexcept;
	HandshakeConnectionException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	HandshakeConnectionException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~HandshakeConnectionException();

	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* PUBSUB_HANDSHAKECONNECTIONEXCEPTION_H_ */
