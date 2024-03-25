/*
 * WrongPubsubConnectionCommandException.h
 *
 *  Created on: 2023/09/11
 *      Author: iizuka
 */

#ifndef P2PSERVER_WRONGPUBSUBCONNECTIONCOMMANDEXCEPTION_H_
#define P2PSERVER_WRONGPUBSUBCONNECTIONCOMMANDEXCEPTION_H_

#include "base/Exception.h"

using namespace alinous;

namespace codablecash {

class WrongPubsubConnectionCommandException : public Exception {
public:
	WrongPubsubConnectionCommandException(const char* srcfile, int srcline) noexcept;
	WrongPubsubConnectionCommandException(Exception* cause, const char* srcfile, int srcline) noexcept;
	WrongPubsubConnectionCommandException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	WrongPubsubConnectionCommandException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~WrongPubsubConnectionCommandException();
	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* P2PSERVER_WRONGPUBSUBCONNECTIONCOMMANDEXCEPTION_H_ */
