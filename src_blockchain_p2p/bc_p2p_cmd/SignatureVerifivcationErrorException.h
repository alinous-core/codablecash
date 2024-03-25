/*
 * SignatureVerifivcationErrorException.h
 *
 *  Created on: 2023/10/02
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_SIGNATUREVERIFIVCATIONERROREXCEPTION_H_
#define BC_P2P_CMD_SIGNATUREVERIFIVCATIONERROREXCEPTION_H_

#include "base/Exception.h"

using namespace alinous;

namespace codablecash {

class SignatureVerifivcationErrorException : public Exception {
public:
	SignatureVerifivcationErrorException(const char* srcfile, int srcline) noexcept;
	SignatureVerifivcationErrorException(Exception* cause, const char* srcfile, int srcline) noexcept;
	SignatureVerifivcationErrorException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	SignatureVerifivcationErrorException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~SignatureVerifivcationErrorException();
	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_SIGNATUREVERIFIVCATIONERROREXCEPTION_H_ */
