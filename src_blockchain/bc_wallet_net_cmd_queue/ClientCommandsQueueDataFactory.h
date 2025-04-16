/*
 * ClientCommandsQueueDataFactory.h
 *
 *  Created on: Mar 26, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_CMD_QUEUE_CLIENTCOMMANDSQUEUEDATAFACTORY_H_
#define BC_WALLET_NET_CMD_QUEUE_CLIENTCOMMANDSQUEUEDATAFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class ClientCommandsQueueDataFactory : public AbstractBtreeDataFactory {
public:
	ClientCommandsQueueDataFactory();
	virtual ~ClientCommandsQueueDataFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);
	virtual AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_CMD_QUEUE_CLIENTCOMMANDSQUEUEDATAFACTORY_H_ */
