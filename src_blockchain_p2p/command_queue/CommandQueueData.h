/*
 * CommandQueueData.h
 *
 *  Created on: 2023/09/25
 *      Author: iizuka
 */

#ifndef COMMAND_QUEUE_COMMANDQUEUEDATA_H_
#define COMMAND_QUEUE_COMMANDQUEUEDATA_H_

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class PubSubId;
class AbstructNodeQueueCommand;

class CommandQueueData : public IBlockObject {
public:
	CommandQueueData(const CommandQueueData& inst);
	CommandQueueData();
	virtual ~CommandQueueData();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static CommandQueueData* createFromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void setPublishId(const PubSubId* pubsubId) noexcept;
	void setCommand(const AbstructNodeQueueCommand* command) noexcept;

	AbstructNodeQueueCommand* getCommand() const noexcept {
		return this->command;
	}
	const PubSubId* getPubSubId() const noexcept {
		return this->pubsubId;
	}

private:
	PubSubId* pubsubId;
	AbstructNodeQueueCommand* command;
};

} /* namespace codablecash */

#endif /* COMMAND_QUEUE_COMMANDQUEUEDATA_H_ */
