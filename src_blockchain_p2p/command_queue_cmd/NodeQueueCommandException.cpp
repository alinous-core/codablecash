/*
 * NodeQueueCommandException.cpp
 *
 *  Created on: 2023/11/11
 *      Author: iizuka
 */

#include <command_queue_cmd/NodeQueueCommandException.h>

#include "base/UnicodeString.h"

using alinous::UnicodeString;

namespace codablecash {

const wchar_t* NodeQueueCommandException::defaultMessage = L"Request processor queue Command Error.";

NodeQueueCommandException::NodeQueueCommandException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
NodeQueueCommandException::NodeQueueCommandException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
NodeQueueCommandException::NodeQueueCommandException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
NodeQueueCommandException::NodeQueueCommandException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}

NodeQueueCommandException::~NodeQueueCommandException() {
}

} /* namespace codablecash */
