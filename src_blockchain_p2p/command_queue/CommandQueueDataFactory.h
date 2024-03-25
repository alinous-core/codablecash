/*
 * CommandQueueDataFactory.h
 *
 *  Created on: 2023/09/19
 *      Author: iizuka
 */

#ifndef COMMAND_QUEUE_COMMANDQUEUEDATAFACTORY_H_
#define COMMAND_QUEUE_COMMANDQUEUEDATAFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class CommandQueueDataFactory : public AbstractBtreeDataFactory {
public:
	CommandQueueDataFactory();
	virtual ~CommandQueueDataFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);
	virtual AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* COMMAND_QUEUE_COMMANDQUEUEDATAFACTORY_H_ */
