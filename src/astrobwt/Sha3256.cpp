/*
 * Sha3256.cpp
 *
 *  Created on: 2022/04/11
 *      Author: iizuka
 */

#include "Sha3256.h"

#include "base_io/ByteBuffer.h"

#include "hash/sha3.h"

namespace codablecash {
/*
ByteBuffer* Sha3_256::sha3_256(ByteBuffer* buff, bool bigEndian) noexcept {
	buff->position(0);

	const char* binary = (const char*)buff->array();
	int length = buff->limit();

	return sha3_256(binary, length, bigEndian);
}*/

ByteBuffer* Sha3_256::sha3_256(const char* binary, int length, bool bigEndian) noexcept {
	sha3_ctx ctx;

	rhash_sha3_256_init(&ctx);
	rhash_sha3_update(&ctx, (const unsigned char*)binary, length);

	uint8_t sha256[32];
	rhash_sha3_final(&ctx, sha256);

	ByteBuffer* buff = ByteBuffer::wrapWithEndian(sha256, 32, bigEndian);
	return buff;
}



} /* namespace codablecash */
