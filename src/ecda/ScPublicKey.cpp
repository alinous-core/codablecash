/*
 * ScPublicKey.cpp
 *
 *  Created on: 2022/03/23
 *      Author: iizuka
 */

#include "ScPublicKey.h"

#include "base/StackRelease.h"

#include "base_io/ByteBuffer.h"

namespace codablecash {

ScPublicKey::ScPublicKey(const Secp256k1Point& pt) : Secp256k1Point(pt.getX(), pt.getY()) {
}

//ScPublicKey::ScPublicKey() : Secp256k1Point() {
//}


ScPublicKey::~ScPublicKey() {

}

int ScPublicKey::binarySize() const {
	int total = sizeof(uint8_t);

	ByteBuffer* b = to65Bytes(); __STP(b);
	total += b->limit();

	return total;
}

void ScPublicKey::toBinary(ByteBuffer* out) const {
	ByteBuffer* b = to65Bytes(); __STP(b);
	out->put(b->limit());

	b->position(0);
	out->put(b);
}

ScPublicKey* ScPublicKey::fromBinary(ByteBuffer* in) {
	uint8_t len = in->get();

	uint8_t* data = new uint8_t[len];
	StackArrayRelease<uint8_t> __data(data);
	Mem::memset(data, 0, len);

	in->get(data, len);

	ByteBuffer* buff = ByteBuffer::wrapWithEndian(data, len, true); __STP(buff);

	buff->position(0);
	Secp256k1Point point = Secp256k1Point::from65Bytes(buff);

	return new ScPublicKey(point);
}

IBlockObject* ScPublicKey::copyData() const noexcept {
	return new ScPublicKey(*this);
}

bool ScPublicKey::equals(const ScPublicKey* other) const noexcept {
	return this->x.compareTo(other->x) == 0 && this->y.compareTo(other->y) == 0;
}


} /* naespace codablecash */
