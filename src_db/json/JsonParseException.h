/*
 * JsonParseException.h
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#ifndef JSON_JSONPARSEEXCEPTION_H_
#define JSON_JSONPARSEEXCEPTION_H_

#include "base/Exception.h"

using namespace alinous;

namespace codablecash {

class JsonParseException : public Exception {
public:
	JsonParseException(const char* srcfile, int srcline) noexcept;
	JsonParseException(Exception* cause, const char* srcfile, int srcline) noexcept;
	JsonParseException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	JsonParseException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~JsonParseException();
	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* JSON_JSONPARSEEXCEPTION_H_ */
