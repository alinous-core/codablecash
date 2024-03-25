/*
 * ClientNotifyNewTransactionCommand.h
 *
 *  Created on: 2023/10/05
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_NOTIFY_CLIENTNOTIFYNEWTRANSACTIONCOMMAND_H_
#define BC_P2P_CMD_CLIENT_NOTIFY_CLIENTNOTIFYNEWTRANSACTIONCOMMAND_H_

#include "bc_p2p_cmd_client_notify/AbstractClientNotifyCommand.h"

namespace codablecash {

class TransactionTransferData;

class ClientNotifyNewTransactionCommand : public AbstractClientNotifyCommand {
public:
	ClientNotifyNewTransactionCommand(const ClientNotifyNewTransactionCommand& inst);
	ClientNotifyNewTransactionCommand();
	virtual ~ClientNotifyNewTransactionCommand();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual void fromBinary(ByteBuffer* buff);

	virtual IBlockObject* copyData() const noexcept;

	void setTransactionTransferData(const TransactionTransferData* data) noexcept;

protected:
	virtual AbstractCommandResponse* executeNotify(const PubSubId *pubsubId, ClientExecutor* clientExec) const;

	virtual ByteBuffer* getSignBinary() const;

private:
	TransactionTransferData* data;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_NOTIFY_CLIENTNOTIFYNEWTRANSACTIONCOMMAND_H_ */
