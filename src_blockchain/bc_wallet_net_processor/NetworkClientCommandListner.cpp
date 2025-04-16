/*
 * NetworkClientCommandListner.cpp
 *
 *  Created on: Mar 21, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_processor/NetworkClientCommandListner.h"
#include "bc_wallet_net_processor/NetworkClientCommandProcessor.h"

#include "base_thread/StackUnlocker.h"
#include "base_thread/SynchronizedLock.h"

#include "bc_wallet_net_cmd/ClientNewTransactionCommand.h"

#include "bc_wallet_net_cmd_queue/ClientCommandsQueueData.h"

#include "data_history_data/TransactionTransferData.h"


namespace codablecash {

NetworkClientCommandListner::NetworkClientCommandListner(NetworkClientCommandProcessor* processor) {
	this->processor = processor;
}

NetworkClientCommandListner::~NetworkClientCommandListner() {
	this->processor = nullptr;
}

void NetworkClientCommandListner::onNewTransaction(const PubSubId *pubsubId, const TransactionTransferData *data) {
	SynchronizedLock* lock = this->processor->getQueueSynchrinizedLock();

	{
		StackUnlocker unlocker(lock, __FILE__, __LINE__);
		if(this->processor->__isSuspended()){
			ClientCommandsQueueData queueData;
			queueData.setData(data);

			this->processor->addPendingQueue(&queueData);
			return;
		}
	}


	ClientNewTransactionCommand clientCommnad;
	clientCommnad.setData(data);
	this->processor->addClientCommand(&clientCommnad);
}

} /* namespace codablecash */
