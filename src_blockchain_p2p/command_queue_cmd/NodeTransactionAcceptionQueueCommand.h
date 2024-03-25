/*
 * NodeTransactionAcceptionQueueCommand.h
 *
 *  Created on: 2023/10/02
 *      Author: iizuka
 */

#ifndef COMMAND_QUEUE_CMD_NODETRANSACTIONACCEPTIONQUEUECOMMAND_H_
#define COMMAND_QUEUE_CMD_NODETRANSACTIONACCEPTIONQUEUECOMMAND_H_

#include "command_queue_cmd/AbstractTransactionAcceptionQueueCommand.h"

namespace codablecash {

class NodeTransactionAcceptionQueueCommand : public AbstractTransactionAcceptionQueueCommand {
public:
	NodeTransactionAcceptionQueueCommand(const NodeTransactionAcceptionQueueCommand& inst);
	NodeTransactionAcceptionQueueCommand();
	virtual ~NodeTransactionAcceptionQueueCommand();

	virtual IBlockObject* copyData() const noexcept;
};

} /* namespace codablecash */

#endif /* COMMAND_QUEUE_CMD_NODETRANSACTIONACCEPTIONQUEUECOMMAND_H_ */
