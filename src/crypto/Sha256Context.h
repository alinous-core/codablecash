/*
 * Sha256Context.h
 *
 *  Created on: 2023/07/03
 *      Author: iizuka
 */

#ifndef CRYPTO_SHA256CONTEXT_H_
#define CRYPTO_SHA256CONTEXT_H_

#include "yescrypt/sha256.h"

namespace codablecash {

class Sha256Context {
public:
	Sha256Context();
	virtual ~Sha256Context();

	void update(const char* binary, int length);
	void finalize();

	const uint8_t* getData32() const noexcept {
		return this->sha256;
	}

private:
	SHA256_CTX ctx;
	uint8_t sha256[32];
};

} /* namespace codablecash */

#endif /* CRYPTO_SHA256CONTEXT_H_ */
