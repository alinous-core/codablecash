/*
 * ClientBlockMinedCommand.h
 *
 *  Created on: Aug 18, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_CMD_CLIENTBLOCKMINEDCOMMAND_H_
#define BC_WALLET_NET_CMD_CLIENTBLOCKMINEDCOMMAND_H_

#include "bc_wallet_net_cmd/AbstractClientQueueCommand.h"

#include "data_history_data/BlockHeaderTransferData.h"


namespace codablecash {

class NodeIdentifier;

class ClientBlockMinedCommand : public AbstractClientQueueCommand {
public:
	ClientBlockMinedCommand(const ClientBlockMinedCommand& inst);
	ClientBlockMinedCommand();
	virtual ~ClientBlockMinedCommand();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void process(NetworkWallet* wallet) const;

	void setData(const BlockHeaderTransferData* data);

	void setSourceNodeId(const NodeIdentifier* nodeId);

private:
	BlockHeaderTransferData* data;
	NodeIdentifier* sourceNodeId;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_CMD_CLIENTBLOCKMINEDCOMMAND_H_ */
