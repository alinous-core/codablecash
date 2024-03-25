/*
 * Sha256Context.cpp
 *
 *  Created on: 2023/07/03
 *      Author: iizuka
 */

#include "crypto/Sha256Context.h"

#include "osenv/memory.h"

using namespace alinous;

namespace codablecash {

Sha256Context::Sha256Context() {
	SHA256_Init(&ctx);

	Mem::memset(this->sha256, 0, 32);
}

Sha256Context::~Sha256Context() {

}

void Sha256Context::update(const char *binary, int length) {
	SHA256_Update(&ctx, binary, length);
}

void Sha256Context::finalize() {
	SHA256_Final((uint8_t *)sha256, &ctx);
}

} /* namespace codablecash */
