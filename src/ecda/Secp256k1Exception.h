/*
 * Secp256k1Exception.h
 *
 *  Created on: 2023/02/03
 *      Author: iizuka
 */

#ifndef ECDA_SECP256K1EXCEPTION_H_
#define ECDA_SECP256K1EXCEPTION_H_

#include "base/Exception.h"

namespace codablecash {

class Secp256k1Exception: public alinous::Exception {
public:
	Secp256k1Exception(const char* srcfile, int srcline) noexcept;
	Secp256k1Exception(Exception* cause, const char* srcfile, int srcline) noexcept;
	Secp256k1Exception(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	Secp256k1Exception(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~Secp256k1Exception();

	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* ECDA_SECP256K1EXCEPTION_H_ */
