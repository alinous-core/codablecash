/*
 * ClientNewTransactionCommand.h
 *
 *  Created on: Apr 15, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_CMD_CLIENTNEWTRANSACTIONCOMMAND_H_
#define BC_WALLET_NET_CMD_CLIENTNEWTRANSACTIONCOMMAND_H_


#include "bc_wallet_net_cmd/AbstractClientQueueCommand.h"

namespace codablecash {

class TransactionTransferData;

class ClientNewTransactionCommand : public AbstractClientQueueCommand {
public:
	ClientNewTransactionCommand(const ClientNewTransactionCommand& inst);
	ClientNewTransactionCommand();
	virtual ~ClientNewTransactionCommand();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void setData(const TransactionTransferData* d);

	virtual void process(NetworkWallet* wallet) const;

private:
	TransactionTransferData *data;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_CMD_CLIENTNEWTRANSACTIONCOMMAND_H_ */
