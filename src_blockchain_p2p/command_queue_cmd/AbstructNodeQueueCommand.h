/*
 * AbstructNodeQueueCommand.h
 *
 *  Created on: 2023/09/29
 *      Author: iizuka
 */

#ifndef COMMAND_QUEUE_CMD_ABSTRUCTNODEQUEUECOMMAND_H_
#define COMMAND_QUEUE_CMD_ABSTRUCTNODEQUEUECOMMAND_H_

#include "filestore_block/IBlockObject.h"

using namespace alinous;


namespace codablecash {

class PubSubId;
class IPubsubCommandListner;

class AbstructNodeQueueCommand : public IBlockObject {
public:
	static const constexpr uint32_t TYPE_NODE_NOP{1};
	static const constexpr uint32_t TYPE_NODE_CLIENT_TRANSACTION_ACCEPT_QUEUE{2};
	static const constexpr uint32_t TYPE_NODE_TRANSACTION_ACCEPT_QUEUE{3};

	AbstructNodeQueueCommand(const AbstructNodeQueueCommand& inst);
	AbstructNodeQueueCommand(uint32_t type);
	virtual ~AbstructNodeQueueCommand();

	virtual void fromBinary(ByteBuffer* buff) = 0;

	virtual void execute(const PubSubId* pubsubId, IPubsubCommandListner* listner) = 0;

	static AbstructNodeQueueCommand* createFromBinary(ByteBuffer* in);

protected:
	uint32_t type;
};

} /* namespace codablecash */

#endif /* COMMAND_QUEUE_CMD_ABSTRUCTNODEQUEUECOMMAND_H_ */
