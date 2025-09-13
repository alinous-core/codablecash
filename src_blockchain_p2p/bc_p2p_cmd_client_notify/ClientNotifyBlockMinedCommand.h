/*
 * ClientNotifyBlockMinedCommand.h
 *
 *  Created on: Aug 18, 2025
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_NOTIFY_CLIENTNOTIFYBLOCKMINEDCOMMAND_H_
#define BC_P2P_CMD_CLIENT_NOTIFY_CLIENTNOTIFYBLOCKMINEDCOMMAND_H_

#include "bc_p2p_cmd_client_notify/AbstractClientNotifyCommand.h"

namespace codablecash {

class BlockHeaderTransferData;

class ClientNotifyBlockMinedCommand : public AbstractClientNotifyCommand {
public:
	ClientNotifyBlockMinedCommand(const ClientNotifyBlockMinedCommand& inst);
	ClientNotifyBlockMinedCommand();
	virtual ~ClientNotifyBlockMinedCommand();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual void fromBinary(ByteBuffer* buff);

	virtual IBlockObject* copyData() const noexcept;

	void setBlockHeaderTransferData(const BlockHeaderTransferData* data);

protected:
	virtual AbstractCommandResponse* executeNotify(const PubSubId *pubsubId, ClientExecutor* clientExec) const;
	virtual ByteBuffer* getSignBinary() const;

private:
	BlockHeaderTransferData* headerData;

};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_NOTIFY_CLIENTNOTIFYBLOCKMINEDCOMMAND_H_ */
