/*
 * RipeMd160.cpp
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */

#include "crypto/RipeMd160.h"

#include "base/StackRelease.h"

#include "base_io/ByteBuffer.h"

#include "ripemd160/ripemd160.h"

#include "osenv/memory.h"

namespace codablecash {

ByteBuffer* RipeMd160::encode(ByteBuffer *inbuff) {
	inbuff->position(0);
	const uint8_t* msg = inbuff->array();
	uint32_t msg_len = inbuff->limit();

	int length = 20;
	uint8_t *hash = new uint8_t[length];
	Mem::memset(hash, 0, length);
	StackArrayRelease<uint8_t> __st_hash(hash);

	ripemd160((const uint8_t*)msg, (uint32_t)msg_len, (uint8_t*)hash);

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(20, true);
	buff->put(hash, length);

	buff->position(0);
	return buff;
}

} /* namespace codablecash */
