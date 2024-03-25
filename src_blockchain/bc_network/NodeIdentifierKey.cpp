/*
 * NodeIdentifierKey.cpp
 *
 *  Created on: 2023/05/09
 *      Author: iizuka
 */

#include "bc_network/NodeIdentifierKey.h"
#include "bc_network/NodeIdentifier.h"
#include "bc_network/NodeIdentifierKeyFactory.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

namespace codablecash {

NodeIdentifierKey::NodeIdentifierKey(const NodeIdentifier *nodeId) {
	this->nodeId = dynamic_cast<NodeIdentifier*>(nodeId->copyData());
}

NodeIdentifierKey::~NodeIdentifierKey() {
	delete this->nodeId;
}

int NodeIdentifierKey::binarySize() const {
	BinaryUtils::checkNotNull(this->nodeId);

	int size = sizeof(uint32_t);
	size += this->nodeId->binarySize();

	return size;
}

void NodeIdentifierKey::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->nodeId);

	out->putInt(NodeIdentifierKeyFactory::NODE_IDENTIFIER_KEY);

	this->nodeId->toBinary(out);
}

NodeIdentifierKey* NodeIdentifierKey::fromBinary(ByteBuffer *in) {
	NodeIdentifier* nodeId = NodeIdentifier::fromBinary(in); __STP(nodeId);
	BinaryUtils::checkNotNull(nodeId);

	return new NodeIdentifierKey(nodeId);
}

int NodeIdentifierKey::compareTo(const AbstractBtreeKey *key) const noexcept {
	if(key->isInfinity()){
		return -1;
	}
	else if(key->isNull()){
		return 1;
	}

	const NodeIdentifierKey* other = dynamic_cast<const NodeIdentifierKey*>(key);
	return this->nodeId->compareTo(other->nodeId);
}

AbstractBtreeKey* NodeIdentifierKey::clone() const noexcept {
	return new NodeIdentifierKey(this->nodeId);
}

} /* namespace codablecash */
