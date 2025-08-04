/*
 * SchnorrKeyPair.cpp
 *
 *  Created on: 2018/04/12
 *      Author: iizuka
 */

#include "debug/debugMacros.h"

#include "crypto/SchnorrKeyPair.h"

#include "base_io/ByteBuffer.h"
#include "base/StackRelease.h"
#include "numeric/BigInteger.h"

namespace codablecash {

SchnorrKeyPair::SchnorrKeyPair() {
	this->secretKey = nullptr;
	this->publicKey = nullptr;
}

SchnorrKeyPair::SchnorrKeyPair(const BigInteger& secretKey, const BigInteger& publicKey) {
	this->secretKey = new BigInteger(secretKey);
	this->publicKey = new BigInteger(publicKey);
}

SchnorrKeyPair::~SchnorrKeyPair() {
	delete this->secretKey;
	delete this->publicKey;
}

IKeyPair* SchnorrKeyPair::clone() const noexcept {
	return new SchnorrKeyPair(*this->secretKey, *this->publicKey);
}

int SchnorrKeyPair::binarySize() const {
	int total = sizeof(int8_t);

	{
		ByteBuffer* p = this->publicKey->toBinary(); __STP(p);
		total += sizeof(int16_t);
		total +=  p->capacity();
	}

	{
		ByteBuffer* p = this->secretKey->toBinary(); __STP(p);
		total += sizeof(int16_t);
		total +=  p->capacity();
	}

	return total;
}

void SchnorrKeyPair::toBinary(ByteBuffer* out) const {
	out->put(IKeyPair::PAIR_SCHNORR);

	{
		ByteBuffer* p = this->publicKey->toBinary(); __STP(p);

		out->putShort(p->capacity());
		out->put(p);
	}

	{
		ByteBuffer* p = this->secretKey->toBinary(); __STP(p);

		out->putShort(p->capacity());
		out->put(p);
	}
}

void SchnorrKeyPair::fromBinary(ByteBuffer* in) {
	{
		int length = in->getShort();
		int position = in->position();

		this->publicKey = BigInteger::fromBinary((const char*)in->array() + position, length);
		in->position(position + length);
	}

	{
		int length = in->getShort();
		int position = in->position();

		this->secretKey = BigInteger::fromBinary((const char*)in->array() + position, length);
		in->position(position + length);
	}
}

const BigInteger* SchnorrKeyPair::getPubKey() const noexcept {
	return this->publicKey;
}

const BigInteger* SchnorrKeyPair::getSecretKey() const noexcept {
	return this->secretKey;
}

} /* namespace codablecash */
