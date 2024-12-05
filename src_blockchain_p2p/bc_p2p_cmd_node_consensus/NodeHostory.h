/*
 * NodeHostory.h
 *
 *  Created on: 2024/05/01
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_CONSENSUS_NODEHOSTORY_H_
#define BC_P2P_CMD_NODE_CONSENSUS_NODEHOSTORY_H_

#include <cstdint>

namespace alinous {
class UnicodeString;
class SystemTimestamp;
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class NodeIdentifier;

class NodeHostory {
public:
	NodeHostory(const NodeHostory& inst);
	NodeHostory(uint16_t zone, const NodeIdentifier* nodeId, const SystemTimestamp *timestamp, int protocol, const UnicodeString* host, uint32_t port);
	virtual ~NodeHostory();

	int binarySize() const;
	void toBinary(ByteBuffer* buff) const;
	static NodeHostory* fromBinary(ByteBuffer* buff);

	void setTimestamp(const SystemTimestamp* tm) noexcept;
	const SystemTimestamp* getTimestamp() const noexcept {
		return this->timestamp;
	}

	const NodeIdentifier* getNodeIdentifier() const noexcept {
		return this->nodeId;
	}

private:
	/***
	 * Refer P2pNodeRecord class
	 */
	uint16_t zone;
	NodeIdentifier* nodeId;
	SystemTimestamp* timestamp;

	/**
	 * defined at P2pNodeRecord
	 * 	static const constexpr int TCP_IP_V4{1};
	 *  static const constexpr int TCP_IP_V6{2};
	 */
	int protocol;
	UnicodeString* host;
	uint32_t port;

};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_CONSENSUS_NODEHOSTORY_H_ */
