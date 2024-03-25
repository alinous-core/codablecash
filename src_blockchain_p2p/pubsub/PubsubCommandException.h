/*
 * PubsubCommandException.h
 *
 *  Created on: 2023/01/13
 *      Author: iizuka
 */

#ifndef PUBSUB_PUBSUBCOMMANDEXCEPTION_H_
#define PUBSUB_PUBSUBCOMMANDEXCEPTION_H_

#include "base/Exception.h"

namespace codablecash {

class PubsubCommandException : public alinous::Exception {
public:
	PubsubCommandException(const char* srcfile, int srcline) noexcept;
	PubsubCommandException(Exception* cause, const char* srcfile, int srcline) noexcept;
	PubsubCommandException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	PubsubCommandException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~PubsubCommandException();

	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* PUBSUB_PUBSUBCOMMANDEXCEPTION_H_ */
