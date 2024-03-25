/*
 * SendTransactionClientCommand.h
 *
 *  Created on: 2023/09/24
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_SENDTRANSACTIONCLIENTCOMMAND_H_
#define BC_P2P_CMD_CLIENT_SENDTRANSACTIONCLIENTCOMMAND_H_

#include "bc_p2p_cmd_client/AbstractClientRequestCommand.h"

namespace codablecash {

class TransactionTransferData;
class AbstractBlockchainTransaction;
class P2pRequestProcessor;
class BlockchainController;
class MemoryPool;

class SendTransactionClientCommand : public AbstractClientRequestCommand {
public:
	SendTransactionClientCommand(const SendTransactionClientCommand& inst);
	SendTransactionClientCommand();
	virtual ~SendTransactionClientCommand();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual void fromBinary(ByteBuffer* buff);

	virtual IBlockObject* copyData() const noexcept;

	void setTransaction(const AbstractBlockchainTransaction* trx);

private:
	bool processTransaction(uint16_t zoneSelf, MemoryPool* memPool, BlockchainController* ctrl) const;

protected:
	virtual AbstractCommandResponse* executeAsClient(ClientNodeHandshake* clientHandshake, CodablecashNodeInstance* inst) const;

	virtual ByteBuffer* getSignBinary() const;

private:
	TransactionTransferData* data;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_SENDTRANSACTIONCLIENTCOMMAND_H_ */
