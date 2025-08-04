/*
 * BloomFilter.cpp
 *
 *  Created on: Mar 28, 2025
 *      Author: iizuka
 */

#include "bc_wallet_filter/BloomFilter.h"

#include "crypto/Sha256.h"

#include "base/StackRelease.h"

#include "base_io/ByteBuffer.h"

namespace codablecash {

BloomFilter::BloomFilter(const BloomFilter &inst) {
	this->byteLength = inst.byteLength;

	this->buffer = new uint8_t[this->byteLength];
	::memcpy(this->buffer, inst.buffer, this->byteLength);


	this->bitlength = inst.bitlength;
}

BloomFilter::BloomFilter(uint16_t byteLength) {
	this->byteLength = byteLength;
	this->buffer = new uint8_t[byteLength]{};

	this->bitlength = byteLength * 8;
}

BloomFilter::~BloomFilter() {
	delete [] this->buffer;
}

void BloomFilter::add(const char *b, int length) {
	ByteBuffer* bb = Sha256::sha256(b, length, true); __STP(bb);
	bb->position(0);

	uint64_t l = this->bitlength;

	uint64_t _v1 = ((uint64_t)bb->getLong());
	uint64_t _v2 = ((uint64_t)bb->getLong());
	uint64_t _v3 = ((uint64_t)bb->getLong());

	uint64_t v1 = _v1 % l;
	uint64_t v2 = _v2 % l;
	uint64_t v3 = _v3 % l;

	setBit(v1);
	setBit(v2);
	setBit(v3);
}

void BloomFilter::setBit(int pos) noexcept {
	int nbytes = pos / 8;
	int shift = pos % 8;

	uint8_t v = this->buffer[nbytes] | ((uint8_t)1 << shift);
	this->buffer[nbytes] = v;
}

bool BloomFilter::hasBit(int pos) {
	int nbytes = pos / 8;
	int shift = pos % 8;

	uint8_t filter = ((uint8_t)1 << shift);
	uint8_t v = this->buffer[nbytes];

	return (v & filter) != 0;
}

bool BloomFilter::checkBytes(const char *b, int length) {
	ByteBuffer* bb = Sha256::sha256(b, length, true); __STP(bb);
	bb->position(0);

	uint64_t l = this->bitlength;

	uint64_t _v1 = ((uint64_t)bb->getLong());
	uint64_t _v2 = ((uint64_t)bb->getLong());
	uint64_t _v3 = ((uint64_t)bb->getLong());

	uint64_t v1 = _v1 % l;
	uint64_t v2 = _v2 % l;
	uint64_t v3 = _v3 % l;

	return checkBytes(v1, v2, v3);
}

bool BloomFilter::checkBytes(uint64_t v1, uint64_t v2, uint64_t v3) {
	bool b1 = hasBit(v1);
	bool b2 = hasBit(v2);
	bool b3 = hasBit(v3);

	return b1 && b2 && b3;
}

int BloomFilter::__binarySize() const {
	int total = sizeof(uint16_t);

	total += this->byteLength; // * sizeof(uint8_t)

	return total;
}

void BloomFilter::__toBinary(ByteBuffer *out) const {
	out->putShort(this->byteLength);

	out->put(this->buffer, this->byteLength);
}

bool BloomFilter::equals(const BloomFilter *other) const noexcept {
	return this->byteLength == other->byteLength && this->bitlength == other->bitlength
			&& filterBinaryEquals(other);
}

bool BloomFilter::filterBinaryEquals(const BloomFilter *other) const noexcept {
	int ret = ::memcmp(this->buffer, other->buffer, this->byteLength);
	return ret == 0;
}

void BloomFilter::__fromBinary(ByteBuffer *in) {
	this->byteLength = in->getShort();
	this->bitlength = this->byteLength * 8;

	in->get(this->buffer, this->byteLength);
}

} /* namespace codablecash */
