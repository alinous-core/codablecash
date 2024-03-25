/*
 * RetryableNode2NodeAccessException.h
 *
 *  Created on: 2023/11/19
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_RETRYABLENODE2NODEACCESSEXCEPTION_H_
#define BC_P2P_CMD_NODE_RETRYABLENODE2NODEACCESSEXCEPTION_H_

#include "base/Exception.h"

using namespace alinous;

namespace codablecash {

class RetryableNode2NodeAccessException : public Exception {
public:
	RetryableNode2NodeAccessException(const char* srcfile, int srcline) noexcept;
	RetryableNode2NodeAccessException(Exception* cause, const char* srcfile, int srcline) noexcept;
	RetryableNode2NodeAccessException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	RetryableNode2NodeAccessException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~RetryableNode2NodeAccessException();
	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_RETRYABLENODE2NODEACCESSEXCEPTION_H_ */
