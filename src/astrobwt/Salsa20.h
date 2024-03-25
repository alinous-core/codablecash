/*
 * Salsa20.h
 *
 *  Created on: 2022/04/11
 *      Author: iizuka
 */

#ifndef ASTROBWT_SALSA20_H_
#define ASTROBWT_SALSA20_H_

#include <cstdint>

#include "base_io/ByteBuffer.h"

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class Xsalsa20 {
public:
	static const char sigma[16];/* {'e', 'x', 'p', 'a',
							  'n', 'd', ' ', '3',
							  '2', '-', 'b', 'y',
							  't', 'e', ' ', 'k'};*/
};

class Salsa20 {
public:
	static const constexpr int32_t crypto_core_salsa20_ref_OUTPUTBYTES{64};
	static const constexpr int32_t crypto_core_salsa20_ref_INPUTBYTES{16};
	static const constexpr int32_t crypto_core_salsa20_ref_KEYBYTES{32};
	static const constexpr int32_t crypto_core_salsa20_ref_CONSTBYTES{16};
	static const constexpr int32_t crypto_stream_salsa20_ref_KEYBYTES{32};
	static const constexpr int32_t crypto_stream_salsa20_ref_NONCEBYTES{8};

	static const constexpr int32_t ROUNDS{20};

	inline static uint64_t rotate(uint32_t u, uint32_t c) noexcept
	{
		return (u << c) | (u >> (32 - c));
	}

	inline static uint32_t load_littleendian(ByteBuffer* x, int offset) {
		return x->getInt(offset);
	}

	inline static void store_littleendian(ByteBuffer* x, int offset, uint32_t u)  {
		x->putInt(offset, u);
	}

	static int crypto_stream_xor(uint8_t* c, const uint8_t* m, int mlen, uint8_t* n, int noffset, const uint8_t* key);

private:
	static int crypto_core(ByteBuffer* outv, ByteBuffer* inv, ByteBuffer* k, ByteBuffer* c);
};

} /* namespace codablecash */

#endif /* ASTROBWT_SALSA20_H_ */
