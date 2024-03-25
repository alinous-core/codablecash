/*
 * PoWSha256Algo.cpp
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#include "pow_hash/PoWSha256Algo.h"

#include "crypto/Sha256.h"

#include "yescrypt/sha256.h"

namespace codablecash {

PoWSha256Algo::PoWSha256Algo()
		: AbstractHashAlgorithmElement() {

}

PoWSha256Algo::~PoWSha256Algo() {

}

void PoWSha256Algo::calulate(const char *in01, const char *in02, const char *in03, char *out) const noexcept {
	SHA256_CTX ctx;
	SHA256_Init(&ctx);

	SHA256_Update(&ctx, in01, 32);
	SHA256_Update(&ctx, in02, 32);
	SHA256_Update(&ctx, in03, 32);

	SHA256_Final((uint8_t *)out, &ctx);
}

} /* namespace codablecash */
