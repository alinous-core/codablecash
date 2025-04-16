/*
 * ClientCommandsQueueFactory.h
 *
 *  Created on: Apr 15, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_CMD_QUEUE_CLIENTCOMMANDSQUEUEFACTORY_H_
#define BC_WALLET_NET_CMD_QUEUE_CLIENTCOMMANDSQUEUEFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class ClientCommandsQueueFactory : public AbstractBtreeDataFactory {
public:
	ClientCommandsQueueFactory();
	virtual ~ClientCommandsQueueFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);
	virtual AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_CMD_QUEUE_CLIENTCOMMANDSQUEUEFACTORY_H_ */
