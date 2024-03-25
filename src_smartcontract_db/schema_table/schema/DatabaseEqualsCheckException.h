/*
 * DatabaseEqualsCheckException.h
 *
 *  Created on: 2023/07/29
 *      Author: iizuka
 */

#ifndef SCHEMA_TABLE_SCHEMA_DATABASEEQUALSCHECKEXCEPTION_H_
#define SCHEMA_TABLE_SCHEMA_DATABASEEQUALSCHECKEXCEPTION_H_

#include "base/Exception.h"

using namespace alinous;

namespace codablecash {

class DatabaseEqualsCheckException : public Exception {
public:
	DatabaseEqualsCheckException(const char* srcfile, int srcline) noexcept;
	DatabaseEqualsCheckException(Exception* cause, const char* srcfile, int srcline) noexcept;
	DatabaseEqualsCheckException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	DatabaseEqualsCheckException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~DatabaseEqualsCheckException();
	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* SCHEMA_TABLE_SCHEMA_DATABASEEQUALSCHECKEXCEPTION_H_ */
