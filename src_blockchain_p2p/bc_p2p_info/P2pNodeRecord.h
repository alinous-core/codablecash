/*
 * P2pNodeRecord.h
 *
 *  Created on: 2023/08/11
 *      Author: iizuka
 */

#ifndef BC_P2P_INFO_P2PNODERECORD_H_
#define BC_P2P_INFO_P2PNODERECORD_H_

#include <cstdint>

#include "base/ArrayList.h"

#include "filestore_block/IBlockObject.h"

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class NodeIdentifier;
class P2PZoneConnectionInfo;

class P2pNodeRecord : public IBlockObject {
public:
	static const constexpr int TCP_IP_V4{1};
	static const constexpr int TCP_IP_V6{2};

	static P2pNodeRecord* createIpV6Record(uint16_t zone, const NodeIdentifier* nodeId, const wchar_t* host, int port);

	P2pNodeRecord(const P2pNodeRecord& inst);
	P2pNodeRecord();
	virtual ~P2pNodeRecord();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static P2pNodeRecord* fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void setNodeId(const NodeIdentifier* nodeId);
	void setAddress(const UnicodeString* host, int port);

	void addConnectedNode(uint16_t zone, const NodeIdentifier* nodeId);
	void removeConnectedNode(uint16_t zone, const NodeIdentifier* nodeId);

	uint16_t getZone() const noexcept {
		return this->zone;
	}
	void setZone(uint16_t zone) noexcept {
		this->zone = zone;
	}
	const UnicodeString* getHost() const noexcept {
		return this->host;
	}
	int getPort() const noexcept {
		return this->port;
	}
	const NodeIdentifier* getNodeIdentifier() const noexcept {
		return this->nodeId;
	}
	int getProtocol() const noexcept {
		return this->protocol;
	}

	void updateLastupdatedTime() noexcept;

	ArrayList<P2PZoneConnectionInfo>* getConnectionZoneList() const noexcept {
		return this->list;
	}

	void stripConnections() noexcept;

	UnicodeString* toString() const noexcept;

private:
	P2PZoneConnectionInfo* getZoneInfo(uint16_t zone) noexcept;

private:
	uint16_t zone;
	NodeIdentifier* nodeId;

	int protocol;
	UnicodeString* host;
	uint32_t port;

	uint64_t lastUpdatedTime;
	uint64_t createdTime;

	ArrayList<P2PZoneConnectionInfo>* list;
};

} /* namespace codablecash */

#endif /* BC_P2P_INFO_P2PNODERECORD_H_ */
