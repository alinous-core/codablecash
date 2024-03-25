/*
 * InvaridTransactionException.h
 *
 *  Created on: 2023/10/11
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_INVALIDTRANSACTIONEXCEPTION_H_
#define BC_P2P_CMD_NODE_INVALIDTRANSACTIONEXCEPTION_H_

#include "base/Exception.h"

using namespace alinous;

namespace codablecash {

class InvalidTransactionException : public Exception {
public:
	InvalidTransactionException(const char* srcfile, int srcline) noexcept;
	InvalidTransactionException(Exception* cause, const char* srcfile, int srcline) noexcept;
	InvalidTransactionException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	InvalidTransactionException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~InvalidTransactionException();
	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_INVALIDTRANSACTIONEXCEPTION_H_ */
