/*
 * PubSubId.cpp
 *
 *  Created on: 2023/01/08
 *      Author: iizuka
 */

#include "pubsub/PubSubId.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"

#include "osenv/funcs.h"

#include "crypto/Sha256.h"

#include "numeric/BigInteger.h"
using namespace alinous;

namespace codablecash {

uint64_t PubSubId::serial = 1;

PubSubId::PubSubId(const PubSubId &inst) {
	int lastPos = inst.id->position();
	inst.id->position(0);

	const uint8_t* binary = inst.id->array();
	int length = inst.id->limit();

	this->id = ByteBuffer::wrapWithEndian(binary, length, true);

	inst.id->position(lastPos);
}

PubSubId::PubSubId(const char* binary, int length) {
	this->id = ByteBuffer::wrapWithEndian((const uint8_t*)binary, length, true);
}

PubSubId::~PubSubId() {
	delete this->id;
}

int PubSubId::binarySize() const {
	int total = sizeof(int32_t);
	total += this->id->capacity();

	return total;
}

void PubSubId::toBinary(ByteBuffer *out) const {
	int cap = this->id->capacity();
	out->putInt(cap);
	out->put(this->id->array(), cap);
}

PubSubId* PubSubId::fromBinary(ByteBuffer *in) {
	int cap = in->getInt();

	uint8_t* buff = new uint8_t[cap];
	StackArrayRelease<uint8_t> __st_buff(buff);

	in->get(buff, cap);

	PubSubId* id = new PubSubId((const char*)buff, cap);
	return id;
}

PubSubId* PubSubId::createNewId() {
	uint64_t tm = Os::getTimestampLong();

	uint64_t binseed[2];
	binseed[0] = tm;
	binseed[1] = serial++;

	ByteBuffer* buff = Sha256::sha256((const char*)binseed, sizeof(uint64_t)*2, true); __STP(buff);
	buff->position(0);
	const char* ar = (const char*)buff->array();
	int size = buff->limit();

	return new PubSubId(ar, size);
}

bool PubSubId::equals(const PubSubId *other) const noexcept {
	return this->id->binaryEquals(other->id);
}

int PubSubId::hashCode() const {
	BigInteger* bint = BigInteger::fromBinary((const char*)this->id->array(), this->id->limit()); __STP(bint);
	int32_t hash = (int32_t)bint->longValue();
	return hash;
}

int PubSubId::ValueCompare::operator ()(const PubSubId *const a, const PubSubId *const b) const {
	return a->id->binaryCmp(b->id);
}


} /* namespace codablecash */
