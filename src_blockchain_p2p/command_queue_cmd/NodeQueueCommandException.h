/*
 * NodeQueueCommandException.h
 *
 *  Created on: 2023/11/11
 *      Author: iizuka
 */

#ifndef COMMAND_QUEUE_CMD_NODEQUEUECOMMANDEXCEPTION_H_
#define COMMAND_QUEUE_CMD_NODEQUEUECOMMANDEXCEPTION_H_

#include "base/Exception.h"

namespace codablecash {

class NodeQueueCommandException : public alinous::Exception {
public:
	NodeQueueCommandException(const char* srcfile, int srcline) noexcept;
	NodeQueueCommandException(Exception* cause, const char* srcfile, int srcline) noexcept;
	NodeQueueCommandException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	NodeQueueCommandException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~NodeQueueCommandException();

	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* COMMAND_QUEUE_CMD_NODEQUEUECOMMANDEXCEPTION_H_ */
