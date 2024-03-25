/*
 * BlockchainStoreException.h
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_BLOCKCHAINSTOREEXCEPTION_H_
#define BC_BLOCKSTORE_BLOCKCHAINSTOREEXCEPTION_H_

#include "base/Exception.h"

namespace codablecash {

class BlockchainStoreException : public alinous::Exception {
public:
	BlockchainStoreException(const char* srcfile, int srcline) noexcept;
	BlockchainStoreException(Exception* cause, const char* srcfile, int srcline) noexcept;
	BlockchainStoreException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	BlockchainStoreException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~BlockchainStoreException();

	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_BLOCKCHAINSTOREEXCEPTION_H_ */
