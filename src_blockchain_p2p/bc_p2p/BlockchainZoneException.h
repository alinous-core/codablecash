/*
 * ZoneNotExistException.h
 *
 *  Created on: 2023/08/11
 *      Author: iizuka
 */

#ifndef BC_P2P_BLOCKCHAINZONEEXCEPTION_H_
#define BC_P2P_BLOCKCHAINZONEEXCEPTION_H_

#include "base/Exception.h"

using namespace alinous;

namespace codablecash {

class BlockchainZoneException : public Exception {
public:
	BlockchainZoneException(const char* srcfile, int srcline) noexcept;
	BlockchainZoneException(Exception* cause, const char* srcfile, int srcline) noexcept;
	BlockchainZoneException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	BlockchainZoneException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~BlockchainZoneException();
	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* BC_P2P_BLOCKCHAINZONEEXCEPTION_H_ */
