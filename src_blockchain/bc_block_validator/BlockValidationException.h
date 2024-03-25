/*
 * BlockValidatorException.h
 *
 *  Created on: 2023/11/27
 *      Author: iizuka
 */

#ifndef BC_BLOCK_VALIDATOR_BLOCKVALIDATIONEXCEPTION_H_
#define BC_BLOCK_VALIDATOR_BLOCKVALIDATIONEXCEPTION_H_

#include "base/Exception.h"

namespace codablecash {

class BlockValidationException : public alinous::Exception {
public:
	BlockValidationException(const char* srcfile, int srcline) noexcept;
	BlockValidationException(Exception* cause, const char* srcfile, int srcline) noexcept;
	BlockValidationException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	BlockValidationException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~BlockValidationException();

	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_VALIDATOR_BLOCKVALIDATIONEXCEPTION_H_ */
