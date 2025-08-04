/*
 * NodeIdentifierSource.cpp
 *
 *  Created on: 2022/02/18
 *      Author: iizuka
 */

#include "bc_network/NodeIdentifierSource.h"

#include "crypto/Schnorr.h"
#include "bc_network/NodeIdentifier.h"

#include "base_io/ByteBuffer.h"
namespace codablecash {

NodeIdentifierSource::NodeIdentifierSource(IKeyPair* pair) {
	this->pair = pair;
}

NodeIdentifierSource::~NodeIdentifierSource() {
	delete this->pair;
}

NodeIdentifierSource* NodeIdentifierSource::create() {
	IKeyPair* pair = Schnorr::generateKey();

	NodeIdentifierSource* src = new NodeIdentifierSource(pair);
	return src;
}

NodeIdentifier NodeIdentifierSource::toNodeIdentifier() const noexcept {
	const BigInteger* pubkey = this->pair->getPubKey();

	return NodeIdentifier(pubkey);
}

int NodeIdentifierSource::binarySize() const {
	int total = this->pair->binarySize();

	return total;
}

void NodeIdentifierSource::toBinary(ByteBuffer* out) const {
	this->pair->toBinary(out);
}

NodeIdentifierSource* NodeIdentifierSource::createFromBinary(ByteBuffer* in) {
	IKeyPair* pair = IKeyPair::createFromBinary(in);

	return new NodeIdentifierSource(pair);
}

IBlockObject* NodeIdentifierSource::copyData() const noexcept {
	return new NodeIdentifierSource(this->pair->clone());
}

const BigInteger* NodeIdentifierSource::getSecretKey() const noexcept {
	return this->pair->getSecretKey();
}

} /* namespace codablecash */
