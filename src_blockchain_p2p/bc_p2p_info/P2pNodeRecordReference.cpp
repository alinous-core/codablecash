/*
 * P2pNodeReference.cpp
 *
 *  Created on: 2023/08/11
 *      Author: iizuka
 */

#include "bc_p2p_info/P2pNodeRecordReference.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

#include "osenv/funcs.h"

namespace codablecash {

P2pNodeRecordReference::P2pNodeRecordReference(const P2pNodeRecordReference &inst) {
	this->nodeId = inst.nodeId != nullptr ? dynamic_cast<NodeIdentifier*>(inst.nodeId->copyData()) : nullptr;

	this->lastUpdatedTime = inst.lastUpdatedTime;
	this->createdTime = inst.createdTime;
}

P2pNodeRecordReference::P2pNodeRecordReference(const NodeIdentifier* nodeId) {
	this->nodeId = dynamic_cast<NodeIdentifier*>(nodeId->copyData());

	this->lastUpdatedTime = Os::getTimestampLong();
	this->createdTime = Os::getTimestampLong();
}

P2pNodeRecordReference::~P2pNodeRecordReference() {
	delete this->nodeId;
}

int P2pNodeRecordReference::binarySize() const {
	BinaryUtils::checkNotNull(this->nodeId);

	int total = this->nodeId->binarySize();
	total += sizeof(this->lastUpdatedTime);
	total += sizeof(this->createdTime);

	return total;
}

void P2pNodeRecordReference::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->nodeId);

	this->nodeId->toBinary(out);
	out->putLong(this->lastUpdatedTime);
	out->putLong(this->createdTime);
}

P2pNodeRecordReference* P2pNodeRecordReference::fromBinary(ByteBuffer *in) {
	NodeIdentifier* nodeId = NodeIdentifier::fromBinary(in); __STP(nodeId);
	BinaryUtils::checkNotNull(nodeId);

	P2pNodeRecordReference* inst = new P2pNodeRecordReference(nodeId);

	inst->lastUpdatedTime = in->getLong();
	inst->createdTime = in->getLong();

	return inst;
}

void P2pNodeRecordReference::updateLastUpdatedTime() noexcept {
	this->lastUpdatedTime = Os::getTimestampLong();
}

} /* namespace codablecash */
