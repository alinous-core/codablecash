/*
 * BlockchainNodeHandshakeException.h
 *
 *  Created on: 2023/11/05
 *      Author: iizuka
 */

#ifndef BC_P2P_BLOCKCHAINNODEHANDSHAKEEXCEPTION_H_
#define BC_P2P_BLOCKCHAINNODEHANDSHAKEEXCEPTION_H_

#include "base/Exception.h"

using namespace alinous;

namespace codablecash {

class BlockchainNodeHandshakeException : public Exception {
public:
	BlockchainNodeHandshakeException(const char* srcfile, int srcline) noexcept;
	BlockchainNodeHandshakeException(Exception* cause, const char* srcfile, int srcline) noexcept;
	BlockchainNodeHandshakeException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	BlockchainNodeHandshakeException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~BlockchainNodeHandshakeException();
	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* BC_P2P_BLOCKCHAINNODEHANDSHAKEEXCEPTION_H_ */
