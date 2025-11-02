/*
 * JsonTokenizeException.h
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#ifndef JSON_JSONTOKENIZEEXCEPTION_H_
#define JSON_JSONTOKENIZEEXCEPTION_H_

#include "base/Exception.h"

using namespace alinous;

namespace codablecash {

class JsonTokenizeException : public Exception {
public:
	JsonTokenizeException(const char* srcfile, int srcline) noexcept;
	JsonTokenizeException(Exception* cause, const char* srcfile, int srcline) noexcept;
	JsonTokenizeException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	JsonTokenizeException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~JsonTokenizeException();
	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* JSON_JSONTOKENIZEEXCEPTION_H_ */
