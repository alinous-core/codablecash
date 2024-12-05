/*
 * NodeIdentifier.cpp
 *
 *  Created on: 2019/01/25
 *      Author: iizuka
 */

#include "NodeIdentifier.h"

#include <algorithm>
#include "crypto/Schnorr.h"
#include "crypto/IKeyPair.h"

#include "base/StackRelease.h"

#include "base_io/ByteBuffer.h"
namespace codablecash {

NodeIdentifier::NodeIdentifier() {
	this->nodeIdentifier = nullptr;
}

NodeIdentifier::NodeIdentifier(const BigInteger* pubkey) {
	this->nodeIdentifier = new BigInteger(*pubkey);
}

NodeIdentifier::~NodeIdentifier() {
	delete this->nodeIdentifier;
}
/*
NodeIdentifier& NodeIdentifier::operator =(NodeIdentifier&& inst) {
	if(this != &inst){
		this->nodeIdentifier = inst.nodeIdentifier;

		inst.nodeIdentifier = nullptr;
	}
	return *this;
}*/
/*
NodeIdentifier::NodeIdentifier(NodeIdentifier&& inst) {
	this->nodeIdentifier = inst.nodeIdentifier;

	inst.nodeIdentifier = nullptr;
}
*/
NodeIdentifier::NodeIdentifier(const NodeIdentifier& inst) {
	this->nodeIdentifier = new BigInteger(*inst.nodeIdentifier);
}

int NodeIdentifier::binarySize() const {
	return sizeof(int32_t) + this->nodeIdentifier->binarySize();
}

void NodeIdentifier::toBinary(ByteBuffer* out) const {
	ByteBuffer* buff2 = this->nodeIdentifier->toBinary(); __STP(buff2);
	out->putInt(buff2->capacity());
	out->put(buff2);
}

NodeIdentifier* NodeIdentifier::fromBinary(ByteBuffer* in) {
	NodeIdentifier* nodeId = new NodeIdentifier();

	{
		int size = in->getInt();
		uint8_t* buff = new uint8_t[size];
		Mem::memset(buff, 0, size);

		StackArrayRelease<uint8_t> st_buff(buff);
		in->get(buff, size);

		nodeId->nodeIdentifier = BigInteger::fromBinary((const char*)buff, size);
	}

	return nodeId;
}

IBlockObject* NodeIdentifier::copyData() const noexcept {
	return new NodeIdentifier(*this);
}

int NodeIdentifier::compareTo(const NodeIdentifier* other) const noexcept {
	return nodeIdentifier->compareTo(*other->nodeIdentifier);
}

int NodeIdentifier::ValueCompare::operator ()(const NodeIdentifier *const a,	const NodeIdentifier *const b) const {
	return a->compareTo(b);
}

int NodeIdentifier::hashCode() const {
	int32_t hash = (int32_t)this->nodeIdentifier->longValue();
	return hash;
}

bool NodeIdentifier::equals(const NodeIdentifier *other) const noexcept {
	return compareTo(other) == 0;
}

} /* namespace codablecash */
