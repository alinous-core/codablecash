/*
 * NopNodeCommand.h
 *
 *  Created on: 2023/09/22
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_NOPNODEQUEUECOMMAND_H_
#define BC_P2P_CMD_NODE_NOPNODEQUEUECOMMAND_H_

#include "command_queue_cmd/AbstructNodeQueueCommand.h"

namespace codablecash {

class NopNodeQueueCommand : public AbstructNodeQueueCommand {
public:
	NopNodeQueueCommand(const NopNodeQueueCommand& inst);
	NopNodeQueueCommand();
	virtual ~NopNodeQueueCommand();

	virtual void execute(const PubSubId* pubsubId, IPubsubCommandListner* listner);
	virtual IBlockObject* copyData() const noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual void fromBinary(ByteBuffer* buff);

	void setNum(int num) noexcept {
		this->num = num;
	}
	int getNum() const noexcept {
		return this->num;
	}

private:
	int num;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_NOPNODEQUEUECOMMAND_H_ */
