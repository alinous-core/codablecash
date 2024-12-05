/*
 * AbstractNodeCommand.h
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_ABSTRACTNODECOMMAND_H_
#define BC_P2P_CMD_NODE_ABSTRACTNODECOMMAND_H_

#include "bc_p2p_cmd/AbstractBlockchainPubsubCommand.h"

namespace codablecash {

class BlockchainNodeHandshake;
class CodablecashNodeInstance;
class P2pRequestProcessor;

class AbstractNodeCommand : public AbstractBlockchainPubsubCommand {
public:
	friend class P2pRequestProcessor;

	AbstractNodeCommand(const AbstractNodeCommand& inst);
	explicit AbstractNodeCommand(uint32_t type);
	virtual ~AbstractNodeCommand();

	virtual AbstractCommandResponse* execute(const PubSubId* pubsubId, IPubsubCommandListner* listner) const;
	virtual bool validateCommand(CodablecashNodeInstance* inst, bool suspend) const;

	virtual bool usePendingQueue() const noexcept;

protected:
	virtual AbstractCommandResponse* executeAsNode(BlockchainNodeHandshake* nodeHandShake, CodablecashNodeInstance* inst, bool suspend) const = 0;

};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_ABSTRACTNODECOMMAND_H_ */
