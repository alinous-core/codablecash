/*
 * NodeHostory.cpp
 *
 *  Created on: 2024/05/01
 *      Author: iizuka
 */

#include "bc_p2p_cmd_node_consensus/NodeHostory.h"

#include "bc_network/NodeIdentifier.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "base_timestamp/SystemTimestamp.h"

#include "bc_base/BinaryUtils.h"


namespace codablecash {


NodeHostory::NodeHostory(const NodeHostory &inst) {
	this->zone = inst.zone;
	this->nodeId = inst.nodeId != nullptr ? dynamic_cast<NodeIdentifier*>(inst.nodeId->copyData()) : nullptr;
	this->timestamp = inst.timestamp != nullptr ? dynamic_cast<SystemTimestamp*>(inst.timestamp->copyData()) : nullptr;

	this->protocol = inst.protocol;
	this->host = new UnicodeString(inst.host);
	this->port = inst.port;
}

NodeHostory::NodeHostory(uint16_t zone, const NodeIdentifier* nodeId,  const SystemTimestamp *timestamp, int protocol, const UnicodeString* host, uint32_t port) {
	this->zone = zone;
	this->nodeId = dynamic_cast<NodeIdentifier*>(nodeId->copyData());
	this->timestamp = new SystemTimestamp(*timestamp);

	this->protocol = protocol;
	this->host = host != nullptr ? new UnicodeString(*host) : new UnicodeString(L"");
	this->port = port;
}

NodeHostory::~NodeHostory() {
	delete this->nodeId;
	delete this->timestamp;

	delete this->host;
}

int NodeHostory::binarySize() const {
	BinaryUtils::checkNotNull(this->nodeId);
	BinaryUtils::checkNotNull(this->timestamp);
	BinaryUtils::checkNotNull(this->host);

	int total = sizeof(uint16_t); // zone
	total += this->nodeId->binarySize();
	total += this->timestamp->binarySize();

	total += sizeof(uint16_t); // protocol
	total += BinaryUtils::stringSize(this->host);
	total += sizeof(uint32_t); // port

	return total;
}

void NodeHostory::toBinary(ByteBuffer *buff) const {
	BinaryUtils::checkNotNull(this->nodeId);
	BinaryUtils::checkNotNull(this->timestamp);
	BinaryUtils::checkNotNull(this->host);

	buff->putShort(this->zone);
	this->nodeId->toBinary(buff);
	this->timestamp->toBinary(buff);

	buff->putShort(this->protocol);
	BinaryUtils::putString(buff, this->host);
	buff->putInt(this->port);
}

NodeHostory* NodeHostory::fromBinary(ByteBuffer *buff) {
	uint16_t zone = buff->getShort();
	NodeIdentifier* nodeId = NodeIdentifier::fromBinary(buff); __STP(nodeId);
	SystemTimestamp* timestamp = SystemTimestamp::fromBinary(buff); __STP(timestamp);

	int protocol = buff->getShort();
	UnicodeString* host = BinaryUtils::getString(buff); __STP(host);
	uint32_t port = buff->getInt();

	NodeHostory* inst = new NodeHostory(zone, nodeId, timestamp, protocol, host, port);
	inst->setTimestamp(timestamp);
	return inst;
}

void NodeHostory::setTimestamp(const SystemTimestamp *tm) noexcept {
	delete this->timestamp;
	this->timestamp = dynamic_cast<SystemTimestamp*>(tm->copyData());
}

} /* namespace codablecash */
