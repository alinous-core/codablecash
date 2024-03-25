/*
 * PoWSalsaAlgo.cpp
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#include "pow_hash/PoWSalsaAlgo.h"

#include "astrobwt/Salsa20.h"

namespace codablecash {

PoWSalsaAlgo::PoWSalsaAlgo()
		: AbstractHashAlgorithmElement() {

}

PoWSalsaAlgo::~PoWSalsaAlgo() {

}

void PoWSalsaAlgo::calulate(const char *in01, const char *in02, const char *in03, char *out) const noexcept {
	Salsa20::crypto_stream_xor((uint8_t*)out, (const uint8_t*)in01, 32, (uint8_t*)in02, 0, (const uint8_t*)in03);
}

} /* namespace codablecash */
