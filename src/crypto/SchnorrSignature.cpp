/*
 * SchnorrSignature.cpp
 *
 *  Created on: 2018/04/12
 *      Author: iizuka
 */

#include "debug/debugMacros.h"

#include "crypto/SchnorrSignature.h"
#include "numeric/BigInteger.h"

#include "base/StackRelease.h"

#include "base_io/ByteBuffer.h"
namespace codablecash {

using namespace std;

SchnorrSignature::SchnorrSignature(const SchnorrSignature& inst) {
	this->e = new BigInteger(*inst.e);
	this->y = new BigInteger(*inst.y);
}

SchnorrSignature::SchnorrSignature(const BigInteger& e, const BigInteger& y) {
	this->e = new BigInteger(e);
	this->y = new BigInteger(y);
}

SchnorrSignature::~SchnorrSignature() {
	delete this->e;
	delete this->y;
}

int SchnorrSignature::binarySize() const {
	int total = sizeof(uint8_t);
	total += this->e->binarySize();

	total += sizeof(uint8_t);
	total += this->y->binarySize();

	return total;
}

void SchnorrSignature::toBinary(ByteBuffer* out) {
	ByteBuffer* ebuff = this->e->toBinary(); __STP(ebuff);
	ebuff->position(0);
	out->put(ebuff->limit());
	out->put(ebuff);

	ByteBuffer* ybuff = this->y->toBinary(); __STP(ybuff);
	ybuff->position(0);
	out->put(ybuff->limit());
	out->put(ybuff);
}

SchnorrSignature* SchnorrSignature::createFromBinary(ByteBuffer* in) {
	BigInteger* e = loadBinary(in); __STP(e);
	BigInteger* y = loadBinary(in); __STP(y);

	return new SchnorrSignature(*e, *y);
}

BigInteger* SchnorrSignature::loadBinary(ByteBuffer* in) {
	int size = (uint8_t)in->get();

	char* buff = new char[size]; StackArrayRelease<char> __st_buff(buff);
	Mem::memset(buff, 0, size);

	in->get((uint8_t*)buff, size);

	return BigInteger::fromBinary(buff, size);
}

} /* namespace codablecash */
