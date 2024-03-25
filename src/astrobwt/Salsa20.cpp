/*
 * Salsa20.cpp
 *
 *  Created on: 2022/04/11
 *      Author: iizuka
 */

#include "Salsa20.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"
namespace codablecash {

const char Xsalsa20::sigma[16]{'e', 'x', 'p', 'a',
	  'n', 'd', ' ', '3',
	  '2', '-', 'b', 'y',
	  't', 'e', ' ', 'k'};

/**
 *
 * c: output -> same size with m
 * m: message
 * mlen: message length
 * n: counter -> 16 bytes value, but use 8 bytes
 * noffset: starting offset of counter
 * k: key -> 32byte(256bit) key
 */
int Salsa20::crypto_stream_xor(uint8_t* c, const uint8_t* m, int mlen, uint8_t* iv, int ivoffset, const uint8_t* key) {
	ByteBuffer* inv = ByteBuffer::allocateWithEndian(16, false); __STP(inv);
	ByteBuffer* block = ByteBuffer::allocateWithEndian(64, false); __STP(block);
	ByteBuffer* k = ByteBuffer::wrapWithEndian(key, 32, false); __STP(k);

	int coffset = 0;
	int moffset = 0;

	//if (mlen == 0){
	//	return 0;
	//}

	for (int i = 0; i < 8; ++i){
		inv->put(i, iv[ivoffset + i]); // inv[i] = n[noffset + i];
	}


	for (int i = 8; i < 16; ++i){
		inv->put(i, 0); // inv[i] = 0;
	}

	ByteBuffer* sigma = ByteBuffer::wrapWithEndian((const uint8_t*)Xsalsa20::sigma, 16, false); __STP(sigma);
	while (mlen >= 64){
		Salsa20::crypto_core(block, inv, k, sigma);

		for (int i = 0; i < 64; ++i){
			c[coffset + i] = (uint8_t)(m[moffset + i] ^ block->get(i));
		}

		uint32_t u = 1;

		for (int i = 8; i < 16; ++i)
		{
			u += inv->get(i) & 0xff;
			inv->put(i, (uint8_t)u); // inv[i] = (uint8_t) u;
			u >>= 8;
		}

		mlen -= 64;
		coffset += 64;
		moffset += 64;
	}

	if (mlen != 0){
		Salsa20::crypto_core(block, inv, k, sigma);

		for (int i = 0; i < mlen; ++i){
				c[coffset + i] = (uint8_t)(m[moffset + i] ^ block->get(i));
		}
	}

	return 0;
}

int Salsa20::crypto_core(ByteBuffer* outv, ByteBuffer* inv, ByteBuffer* k, ByteBuffer* c) {
	uint32_t x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15;
	uint32_t j0, j1, j2, j3, j4, j5, j6, j7, j8, j9, j10, j11, j12, j13, j14, j15;
	//uint32_t x0{0}, x1{0}, x2{0}, x3{0}, x4{0}, x5{0}, x6{0}, x7{0}, x8{0}, x9{0}, x10{0}, x11{0}, x12{0}, x13{0}, x14{0}, x15{0};
	//uint32_t j0{0}, j1{0}, j2{0}, j3{0}, j4{0}, j5{0}, j6{0}, j7{0}, j8{0}, j9{0}, j10{0}, j11{0}, j12{0}, j13{0}, j14{0}, j15{0};

	j0 = x0 = load_littleendian(c, 0);
	j1 = x1 = load_littleendian(k, 0);
	j2 = x2 = load_littleendian(k, 4);
	j3 = x3 = load_littleendian(k, 8);
	j4 = x4 = load_littleendian(k, 12);
	j5 = x5 = load_littleendian(c, 4);
	j6 = x6 = load_littleendian(inv, 0);
	j7 = x7 = load_littleendian(inv, 4);
	j8 = x8 = load_littleendian(inv, 8);
	j9 = x9 = load_littleendian(inv, 12);
	j10 = x10 = load_littleendian(c, 8);
	j11 = x11 = load_littleendian(k, 16);
	j12 = x12 = load_littleendian(k, 20);
	j13 = x13 = load_littleendian(k, 24);
	j14 = x14 = load_littleendian(k, 28);
	j15 = x15 = load_littleendian(c, 12);

	for (int i = ROUNDS; i > 0; i -= 2)
	{
		x4 ^= rotate(x0 + x12, 7);
		x8 ^= rotate(x4 + x0, 9);
		x12 ^= rotate(x8 + x4, 13);
		x0 ^= rotate(x12 + x8, 18);
		x9 ^= rotate(x5 + x1, 7);
		x13 ^= rotate(x9 + x5, 9);
		x1 ^= rotate(x13 + x9, 13);
		x5 ^= rotate(x1 + x13, 18);
		x14 ^= rotate(x10 + x6, 7);
		x2 ^= rotate(x14 + x10, 9);
		x6 ^= rotate(x2 + x14, 13);
		x10 ^= rotate(x6 + x2, 18);
		x3 ^= rotate(x15 + x11, 7);
		x7 ^= rotate(x3 + x15, 9);
		x11 ^= rotate(x7 + x3, 13);
		x15 ^= rotate(x11 + x7, 18);
		x1 ^= rotate(x0 + x3, 7);
		x2 ^= rotate(x1 + x0, 9);
		x3 ^= rotate(x2 + x1, 13);
		x0 ^= rotate(x3 + x2, 18);
		x6 ^= rotate(x5 + x4, 7);
		x7 ^= rotate(x6 + x5, 9);
		x4 ^= rotate(x7 + x6, 13);
		x5 ^= rotate(x4 + x7, 18);
		x11 ^= rotate(x10 + x9, 7);
		x8 ^= rotate(x11 + x10, 9);
		x9 ^= rotate(x8 + x11, 13);
		x10 ^= rotate(x9 + x8, 18);
		x12 ^= rotate(x15 + x14, 7);
		x13 ^= rotate(x12 + x15, 9);
		x14 ^= rotate(x13 + x12, 13);
		x15 ^= rotate(x14 + x13, 18);
	}

	x0 += j0;
	x1 += j1;
	x2 += j2;
	x3 += j3;
	x4 += j4;
	x5 += j5;
	x6 += j6;
	x7 += j7;
	x8 += j8;
	x9 += j9;
	x10 += j10;
	x11 += j11;
	x12 += j12;
	x13 += j13;
	x14 += j14;
	x15 += j15;


	store_littleendian(outv, 0, x0);
	store_littleendian(outv, 4, x1);
	store_littleendian(outv, 8, x2);
	store_littleendian(outv, 12, x3);
	store_littleendian(outv, 16, x4);
	store_littleendian(outv, 20, x5);
	store_littleendian(outv, 24, x6);
	store_littleendian(outv, 28, x7);
	store_littleendian(outv, 32, x8);
	store_littleendian(outv, 36, x9);
	store_littleendian(outv, 40, x10);
	store_littleendian(outv, 44, x11);
	store_littleendian(outv, 48, x12);
	store_littleendian(outv, 52, x13);
	store_littleendian(outv, 56, x14);
	store_littleendian(outv, 60, x15);

	return 0;
}

} /* namespace codablecash */
