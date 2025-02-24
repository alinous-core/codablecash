/*
 * InvalidZoneException.h
 *
 *  Created on: Dec 21, 2024
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_INVALIDZONEEXCEPTION_H_
#define BC_BLOCKSTORE_INVALIDZONEEXCEPTION_H_

#include "base/Exception.h"

namespace codablecash {
using namespace alinous;

class InvalidZoneException : public Exception{
public:
	InvalidZoneException(const char* srcfile, int srcline) noexcept;
	InvalidZoneException(Exception* cause, const char* srcfile, int srcline) noexcept;
	InvalidZoneException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	InvalidZoneException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~InvalidZoneException();
	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_INVALIDZONEEXCEPTION_H_ */
