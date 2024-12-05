/*
 * SendTransactionNodeCommand.h
 *
 *  Created on: 2023/10/01
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_SENDTRANSACTIONNODECOMMAND_H_
#define BC_P2P_CMD_NODE_SENDTRANSACTIONNODECOMMAND_H_

#include "bc_p2p_cmd_node/AbstractNodeCommand.h"

namespace codablecash {

class TransactionTransferData;
class MemoryPool;
class BlockchainController;

class SendTransactionNodeCommand : public AbstractNodeCommand {
public:
	SendTransactionNodeCommand(const SendTransactionNodeCommand& inst);
	SendTransactionNodeCommand();
	virtual ~SendTransactionNodeCommand();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual void fromBinary(ByteBuffer* buff);

	virtual ByteBuffer* getSignBinary() const;
	virtual IBlockObject* copyData() const noexcept;

	virtual AbstractCommandResponse* executeAsNode(BlockchainNodeHandshake* nodeHandShake, CodablecashNodeInstance* inst, bool suspend) const;

	void setTransactionTransferData(const TransactionTransferData* data) noexcept;

private:
	bool processTransaction(uint16_t zoneSelf, MemoryPool* memPool, BlockchainController* ctrl) const;

private:
	TransactionTransferData* data;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_SENDTRANSACTIONNODECOMMAND_H_ */
