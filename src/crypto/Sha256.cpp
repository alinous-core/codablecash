/*
 * Sha256.cpp
 *
 *  Created on: 2019/01/05
 *      Author: iizuka
 */

#include "crypto/Sha256.h"
#include "base_io/ByteBuffer.h"

#include "yescrypt/sha256.h"

namespace codablecash {

ByteBuffer* Sha256::sha256(ByteBuffer* buff, bool bigEndian) noexcept {
	buff->position(0);

	const char* binary = (const char*)buff->array();
	int length = buff->limit();

	return sha256(binary, length, bigEndian);
}


ByteBuffer* Sha256::sha256(const char* binary, int length, bool bigEndian) noexcept {
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, binary, length);

	uint8_t sha256[32];
	SHA256_Final((uint8_t *)sha256, &ctx);

	ByteBuffer* buff = ByteBuffer::wrapWithEndian(sha256, 32, bigEndian);
	return buff;
}

} /* namespace codablecash */
