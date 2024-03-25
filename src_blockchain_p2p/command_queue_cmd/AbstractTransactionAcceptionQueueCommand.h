/*
 * AbstractTransactionAcceptionQueueCommand.h
 *
 *  Created on: 2023/10/02
 *      Author: iizuka
 */

#ifndef COMMAND_QUEUE_CMD_ABSTRACTTRANSACTIONACCEPTIONQUEUECOMMAND_H_
#define COMMAND_QUEUE_CMD_ABSTRACTTRANSACTIONACCEPTIONQUEUECOMMAND_H_

#include "command_queue_cmd/AbstructNodeQueueCommand.h"

namespace codablecash {

class TransactionTransferData;

class AbstractTransactionAcceptionQueueCommand : public AbstructNodeQueueCommand {
public:
	AbstractTransactionAcceptionQueueCommand(const AbstractTransactionAcceptionQueueCommand& inst);
	AbstractTransactionAcceptionQueueCommand(uint32_t type);
	virtual ~AbstractTransactionAcceptionQueueCommand();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual void fromBinary(ByteBuffer* buff);

	void setData(const TransactionTransferData* data) noexcept;

	virtual void execute(const PubSubId* pubsubId, IPubsubCommandListner* listner);

private:
	TransactionTransferData* data;
};

} /* namespace codablecash */

#endif /* COMMAND_QUEUE_CMD_ABSTRACTTRANSACTIONACCEPTIONQUEUECOMMAND_H_ */
