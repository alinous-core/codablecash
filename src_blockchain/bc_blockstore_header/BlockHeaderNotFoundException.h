/*
 * BlockHeaderNotFoundException.h
 *
 *  Created on: 2023/12/18
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_HEADER_BLOCKHEADERNOTFOUNDEXCEPTION_H_
#define BC_BLOCKSTORE_HEADER_BLOCKHEADERNOTFOUNDEXCEPTION_H_

#include "base/Exception.h"

namespace codablecash {

class BlockHeaderNotFoundException : public alinous::Exception {
public:
	BlockHeaderNotFoundException(const char* srcfile, int srcline) noexcept;
	BlockHeaderNotFoundException(Exception* cause, const char* srcfile, int srcline) noexcept;
	BlockHeaderNotFoundException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	BlockHeaderNotFoundException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~BlockHeaderNotFoundException();
	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_HEADER_BLOCKHEADERNOTFOUNDEXCEPTION_H_ */
