/*
 * NodeHostory.cpp
 *
 *  Created on: 2024/05/01
 *      Author: iizuka
 */

#include "bc_network/NodeIdentifier.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "base_timestamp/SystemTimestamp.h"

#include "bc_base/BinaryUtils.h"
#include "bc_p2p_cmd_network/NodeNetworkInfo.h"

#include "bc_p2p_info/P2pNodeRecord.h"

#include "ipconnect/IpV4ClientConnection.h"
#include "ipconnect/IpV6ClientConnection.h"

#include "bc/ExceptionThrower.h"

#include "ipconnect/UnexpectedProtocolException.h"
namespace codablecash {


NodeNetworkInfo::NodeNetworkInfo(const NodeNetworkInfo &inst) {
	this->zone = inst.zone;
	this->nodeId = inst.nodeId != nullptr ? dynamic_cast<NodeIdentifier*>(inst.nodeId->copyData()) : nullptr;
	this->canonicalName = inst.canonicalName != nullptr ? new UnicodeString(inst.canonicalName) : nullptr;

	this->timestamp = inst.timestamp != nullptr ? dynamic_cast<SystemTimestamp*>(inst.timestamp->copyData()) : nullptr;

	this->protocol = inst.protocol;
	this->host = new UnicodeString(inst.host);
	this->port = inst.port;
}

NodeNetworkInfo::NodeNetworkInfo(uint16_t zone, const NodeIdentifier* nodeId,  const SystemTimestamp *timestamp, int protocol, const UnicodeString* host, uint32_t port) {
	this->zone = zone;
	this->nodeId = dynamic_cast<NodeIdentifier*>(nodeId->copyData());
	this->canonicalName = nullptr;
	this->timestamp = new SystemTimestamp(*timestamp);

	this->protocol = protocol;
	this->host = host != nullptr ? new UnicodeString(*host) : new UnicodeString(L"");
	this->port = port;
}

NodeNetworkInfo::~NodeNetworkInfo() {
	delete this->nodeId;
	delete this->canonicalName;
	delete this->timestamp;

	delete this->host;
}

void NodeNetworkInfo::setCanonicalName(const UnicodeString *str) noexcept {
	delete this->canonicalName;
	this->canonicalName = str != nullptr ? new UnicodeString(str) : nullptr;
}

int NodeNetworkInfo::binarySize() const {
	BinaryUtils::checkNotNull(this->nodeId);
	BinaryUtils::checkNotNull(this->timestamp);
	BinaryUtils::checkNotNull(this->host);

	int total = sizeof(uint16_t); // zone
	total += this->nodeId->binarySize();

	total += sizeof(uint8_t);
	if(this->canonicalName != nullptr){
		total += BinaryUtils::stringSize(this->canonicalName);
	}

	total += this->timestamp->binarySize();

	total += sizeof(uint16_t); // protocol
	total += BinaryUtils::stringSize(this->host);
	total += sizeof(uint32_t); // port

	return total;
}

void NodeNetworkInfo::toBinary(ByteBuffer *buff) const {
	BinaryUtils::checkNotNull(this->nodeId);
	BinaryUtils::checkNotNull(this->timestamp);
	BinaryUtils::checkNotNull(this->host);

	buff->putShort(this->zone);
	this->nodeId->toBinary(buff);

	uint8_t bl = (this->canonicalName != nullptr) ? 1 : 0;
	buff->put(bl);
	if(bl > 0){
		BinaryUtils::putString(buff, this->canonicalName);
	}

	this->timestamp->toBinary(buff);

	buff->putShort(this->protocol);
	BinaryUtils::putString(buff, this->host);
	buff->putInt(this->port);
}

NodeNetworkInfo* NodeNetworkInfo::fromBinary(ByteBuffer *buff) {
	uint16_t zone = buff->getShort();
	NodeIdentifier* nodeId = NodeIdentifier::fromBinary(buff); __STP(nodeId);

	UnicodeString* canonicalName = nullptr;
	uint8_t bl = buff->get();
	if(bl > 0){
		canonicalName = BinaryUtils::getString(buff);
	}
	__STP(canonicalName);

	SystemTimestamp* timestamp = SystemTimestamp::fromBinary(buff); __STP(timestamp);

	int protocol = buff->getShort();
	UnicodeString* host = BinaryUtils::getString(buff); __STP(host);
	uint32_t port = buff->getInt();

	NodeNetworkInfo* inst = new NodeNetworkInfo(zone, nodeId, timestamp, protocol, host, port);
	inst->setTimestamp(timestamp);
	inst->setCanonicalName(canonicalName);

	return inst;
}

void NodeNetworkInfo::setTimestamp(const SystemTimestamp *tm) noexcept {
	delete this->timestamp;
	this->timestamp = dynamic_cast<SystemTimestamp*>(tm->copyData());
}

IClientSocket* NodeNetworkInfo::getConnection() {
	IClientSocket* con = nullptr;

	if(this->protocol == P2pNodeRecord::TCP_IP_V4){
		con = new IpV4ClientConnection();
	}
	else if(this->protocol == P2pNodeRecord::TCP_IP_V6){
		con = new IpV6ClientConnection();
	}
	__STP(con);

	ExceptionThrower<UnexpectedProtocolException>::throwExceptionIfCondition(con == nullptr, L"Unsupported Protocol", __FILE__, __LINE__);

	con->connect(this->host, this->port);

	return __STP_MV(con);
}

P2pNodeRecord* NodeNetworkInfo::toP2pNodeRecord() const noexcept {
	P2pNodeRecord* record = P2pNodeRecord::createIpV6Record(this->zone, this->nodeId, this->canonicalName, this->host, this->port); __STP(record);

	return __STP_MV(record);
}
} /* namespace codablecash */
