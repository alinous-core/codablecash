/*
 * ClientTransactionAcceptionQueueCommand.h
 *
 *  Created on: 2023/09/29
 *      Author: iizuka
 */

#ifndef COMMAND_QUEUE_CMD_CLIENTTRANSACTIONACCEPTIONQUEUECOMMAND_H_
#define COMMAND_QUEUE_CMD_CLIENTTRANSACTIONACCEPTIONQUEUECOMMAND_H_

#include "command_queue_cmd/AbstractTransactionAcceptionQueueCommand.h"


namespace codablecash {

class TransactionTransferData;

class ClientTransactionAcceptionQueueCommand : public AbstractTransactionAcceptionQueueCommand {
public:
	ClientTransactionAcceptionQueueCommand(const ClientTransactionAcceptionQueueCommand& inst);
	ClientTransactionAcceptionQueueCommand();
	virtual ~ClientTransactionAcceptionQueueCommand();

	virtual IBlockObject* copyData() const noexcept;

};

} /* namespace codablecash */

#endif /* COMMAND_QUEUE_CMD_CLIENTTRANSACTIONACCEPTIONQUEUECOMMAND_H_ */
