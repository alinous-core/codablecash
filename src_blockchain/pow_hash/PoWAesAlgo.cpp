/*
 * PoWAesAlgo.cpp
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#include "pow_hash/PoWAesAlgo.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "bc_base/Base64.h"

#include "osenv/memory.h"

#include "crypto/Aes256Cbc.h"

namespace codablecash {

PoWAesAlgo::PoWAesAlgo() : AbstractHashAlgorithmElement() {

}

PoWAesAlgo::~PoWAesAlgo() {

}

void PoWAesAlgo::calulate(const char *in01, const char *in02, const char *in03, char *out) const noexcept {
	UnicodeString* str = Base64::encode((const uint8_t*)in01, 32); __STP(str);

	Aes256Cbc aes;
	aes.setIv((uint8_t*)in02, 16);
	aes.setKey((uint8_t*)in03);

	Aes256CbcResult* res = aes.encryptoPlainText(str); __STP(res);

	Mem::memcpy(out, res->data, 32);
}

} /* namespace codablecash */
