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
#include "bc_wallet_net_cmd/ClientBlockMinedCommand.h"

#include "bc_wallet_net_cmd_queue/ClientCommandsQueueData.h"

#include "data_history_data/TransactionTransferData.h"
#include "data_history_data/BlockHeaderTransferData.h"

#include "bc_wallet_net/WalletNetworkManager.h"

#include "bc/ExceptionThrower.h"

#include "bc_p2p/BlockchainNodeHandshakeException.h"


namespace codablecash {

NetworkClientCommandListner::NetworkClientCommandListner(NetworkClientCommandProcessor* processor) {
	this->processor = processor;
}

NetworkClientCommandListner::~NetworkClientCommandListner() {
	this->processor = nullptr;
}

void NetworkClientCommandListner::onNewTransaction(const PubSubId *pubsubId, const TransactionTransferData *data) {
	SynchronizedLock* lock = this->processor->getQueueSynchrinizedLock();

	// add handshake info
	WalletNetworkManager* networkManager = this->processor->getWalletNetworkManager();
	const NodeIdentifier* nodeId = networkManager->pubsubId2NodeId(pubsubId);

	// nodeId == nullptr (handshake ends)
	ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(nodeId == nullptr, L"Node connection has alrealy closed.", __FILE__, __LINE__);

	{
		StackUnlocker unlocker(lock, __FILE__, __LINE__);
		if(this->processor->__isSuspended()){
			ClientCommandsQueueData queueData;
			queueData.setData(data);
			queueData.setSourceNodeId(nodeId);

			this->processor->addPendingQueue(&queueData);
			return;
		}
	}


	ClientNewTransactionCommand clientCommnad;
	clientCommnad.setData(data);
	this->processor->addClientCommand(&clientCommnad);
}

void NetworkClientCommandListner::onBlockMined(const PubSubId *pubsubId, const BlockHeaderTransferData *data) {
	SynchronizedLock* lock = this->processor->getQueueSynchrinizedLock();

	// add handshake info
	WalletNetworkManager* networkManager = this->processor->getWalletNetworkManager();
	const NodeIdentifier* nodeId = networkManager->pubsubId2NodeId(pubsubId);

	// nodeId == nullptr (handshake ends)
	ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(nodeId == nullptr, L"Node connection has alrealy closed.", __FILE__, __LINE__);

	{
		StackUnlocker unlocker(lock, __FILE__, __LINE__);
		if(this->processor->__isSuspended()){
			ClientCommandsQueueData queueData;
			queueData.setData(data);
			queueData.setSourceNodeId(nodeId);

			this->processor->addPendingQueue(&queueData);
			return;
		}
	}

	ClientBlockMinedCommand clientCommnad;
	clientCommnad.setData(data);
	clientCommnad.setSourceNodeId(nodeId);
	this->processor->addClientCommand(&clientCommnad);


}

} /* namespace codablecash */
