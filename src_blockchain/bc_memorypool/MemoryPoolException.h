/*
 * MemoryPoolException.h
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#ifndef BC_MEMORYPOOL_MEMORYPOOLEXCEPTION_H_
#define BC_MEMORYPOOL_MEMORYPOOLEXCEPTION_H_

#include "base/Exception.h"

namespace codablecash {

class MemoryPoolException : public alinous::Exception {
public:
	MemoryPoolException(const char* srcfile, int srcline) noexcept;
	MemoryPoolException(Exception* cause, const char* srcfile, int srcline) noexcept;
	MemoryPoolException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	MemoryPoolException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~MemoryPoolException();

	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* BC_MEMORYPOOL_MEMORYPOOLEXCEPTION_H_ */
