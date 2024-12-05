/*
 * SendVoteTransactionNodeCommand.h
 *
 *  Created on: 2024/07/24
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_CONSENSUS_SENDVOTETRANSACTIONNODECOMMAND_H_
#define BC_P2P_CMD_NODE_CONSENSUS_SENDVOTETRANSACTIONNODECOMMAND_H_

#include "bc_p2p_cmd_node_consensus/AbstractConsensusNodeCommand.h"

namespace codablecash {

class TransactionTransferData;
class MemoryPool;
class BlockchainController;

class SendVoteTransactionNodeCommand : public AbstractConsensusNodeCommand {
public:
	SendVoteTransactionNodeCommand(const SendVoteTransactionNodeCommand& inst);
	SendVoteTransactionNodeCommand();
	virtual ~SendVoteTransactionNodeCommand();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual void fromBinary(ByteBuffer* buff);

	virtual ByteBuffer* getSignBinary() const;
	virtual IBlockObject* copyData() const noexcept;

	virtual SystemTimestamp getFirstTimestamp() const;

	void setTransactionTransferData(const TransactionTransferData* data) noexcept;

protected:
	virtual AbstractCommandResponse* executeAsNode(BlockchainNodeHandshake* nodeHandShake, CodablecashNodeInstance* inst, bool suspend) const;

private:
	bool processTransaction(uint16_t zoneSelf, MemoryPool* memPool, BlockchainController* ctrl) const;

private:
	TransactionTransferData* data;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_CONSENSUS_SENDVOTETRANSACTIONNODECOMMAND_H_ */
