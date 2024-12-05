/*
 * PubsubNetworkException.h
 *
 *  Created on: Sep 8, 2024
 *      Author: iizuka
 */

#ifndef PUBSUB_PUBSUBNETWORKEXCEPTION_H_
#define PUBSUB_PUBSUBNETWORKEXCEPTION_H_

#include "base/Exception.h"

namespace codablecash {

class PubsubNetworkException  : public alinous::Exception {
public:
	PubsubNetworkException(const char* srcfile, int srcline) noexcept;
	PubsubNetworkException(Exception* cause, const char* srcfile, int srcline) noexcept;
	PubsubNetworkException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	PubsubNetworkException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~PubsubNetworkException();

	static const wchar_t* defaultMessage;
};

}

#endif /* PUBSUB_PUBSUBNETWORKEXCEPTION_H_ */
