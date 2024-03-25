/*
 * BlockIdChangedException.h
 *
 *  Created on: 2023/03/31
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_BLOCKIDCHANGEDEXCEPTION_H_
#define BC_STATUS_CACHE_BLOCKIDCHANGEDEXCEPTION_H_

#include "base/Exception.h"

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class BlockIdChangedException : public alinous::Exception {
public:
	BlockIdChangedException(const char* srcfile, int srcline) noexcept;
	BlockIdChangedException(Exception* cause, const char* srcfile, int srcline) noexcept;
	BlockIdChangedException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	BlockIdChangedException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~BlockIdChangedException();

	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_BLOCKIDCHANGEDEXCEPTION_H_ */
