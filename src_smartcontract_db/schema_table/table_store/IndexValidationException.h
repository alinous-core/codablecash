/*
 * IndexValidationException.h
 *
 *  Created on: 2023/07/27
 *      Author: iizuka
 */

#ifndef SCHEMA_TABLE_TABLE_STORE_INDEXVALIDATIONEXCEPTION_H_
#define SCHEMA_TABLE_TABLE_STORE_INDEXVALIDATIONEXCEPTION_H_

#include "base/Exception.h"

using namespace alinous;

namespace codablecash {

class IndexValidationException : public Exception {
public:
	IndexValidationException(const char* srcfile, int srcline) noexcept;
	IndexValidationException(Exception* cause, const char* srcfile, int srcline) noexcept;
	IndexValidationException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	IndexValidationException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~IndexValidationException();

	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* SCHEMA_TABLE_TABLE_STORE_INDEXVALIDATIONEXCEPTION_H_ */
