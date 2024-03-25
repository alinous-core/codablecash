/*
 * AlreadyFinalizedException.h
 *
 *  Created on: 2023/05/29
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_CONTEXT_FINALIZER_ALREADYFINALIZEDEXCEPTION_H_
#define BC_STATUS_CACHE_CONTEXT_FINALIZER_ALREADYFINALIZEDEXCEPTION_H_

#include "base/Exception.h"

namespace codablecash {

class AlreadyFinalizedException : public alinous::Exception {
public:
	AlreadyFinalizedException(const char* srcfile, int srcline) noexcept;
	AlreadyFinalizedException(Exception* cause, const char* srcfile, int srcline) noexcept;
	AlreadyFinalizedException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	AlreadyFinalizedException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~AlreadyFinalizedException();

	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_CONTEXT_FINALIZER_ALREADYFINALIZEDEXCEPTION_H_ */
