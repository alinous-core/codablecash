/*
 * NodeShutdownCommand.h
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NETWORK_NODESHUTDOWNCOMMAND_H_
#define BC_P2P_CMD_NETWORK_NODESHUTDOWNCOMMAND_H_

#include "bc_p2p_cmd_network/AbstractNodeNetworkNotifyCommand.h"

namespace alinous {
class SystemTimestamp;
}
using namespace alinous;

namespace codablecash {

class NodeIdentifier;
class CodablecashNodeInstance;

class NodeShutdownCommand : public AbstractNodeNetworkNotifyCommand {
public:
	NodeShutdownCommand(const NodeShutdownCommand& inst);
	NodeShutdownCommand();
	virtual ~NodeShutdownCommand();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual void fromBinary(ByteBuffer* buff);

	virtual IBlockObject* copyData() const noexcept;

	void signTimestamp(const NodeIdentifierSource *source);
	bool verifyTimestamp() const;

	void incHpos() noexcept {
		this->hops++;
	}

protected:
	virtual ByteBuffer* getSignBinary() const;
	ByteBuffer* getSignTimestampBinary() const;

	virtual AbstractCommandResponse* executeAsNode(BlockchainNodeHandshake* nodeHandShake, CodablecashNodeInstance* inst, bool suspend) const;

private:
	bool updateShutdownNode(CodablecashNodeInstance* inst) const;

private:
	uint8_t hops;
	NodeIdentifier *targetNodeId;
	SystemTimestamp* tm;
	SchnorrSignature* tmSig;

};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NETWORK_NODESHUTDOWNCOMMAND_H_ */
