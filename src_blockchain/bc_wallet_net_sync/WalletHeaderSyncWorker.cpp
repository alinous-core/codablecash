/*
 * WalletHeaderSyncWorker.cpp
 *
 *  Created on: Apr 21, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_sync/WalletHeaderSyncWorker.h"
#include "bc_wallet_net_sync/NetworkWalletSync.h"

#include "bc_wallet_net/WalletNetworkManager.h"

#include "bc_network/NodeIdentifier.h"

#include "bc/ExceptionThrower.h"
#include "bc/ISystemLogger.h"

#include "bc_p2p/BlockchainNodeHandshakeException.h"
#include "bc_p2p/StackHandshakeReleaser.h"
#include "bc_p2p/ClientNodeHandshake.h"

#include "bc_p2p_cmd_node/SyncHeaderHeightData.h"

#include "base/StackRelease.h"

#include "bc_p2p_cmd_client/ClientSyncHeaderCommand.h"
#include "bc_p2p_cmd_client/ClientSyncHeaderCommandResponse.h"
#include "bc_p2p_cmd_client/ClientSyncHeaderHeightData.h"


namespace codablecash {

WalletHeaderSyncWorker::WalletHeaderSyncWorker(uint16_t zone, const NodeIdentifier* nodeId, uint64_t startHeight, NetworkWalletSync* parent, ISystemLogger* logger, const UnicodeString* name)
		: AbstractSyncWorkerThread(logger, name) {
	this->nodeId = new NodeIdentifier(*nodeId);
	this->startHeight = startHeight;
	this->parent = parent;
	this->zone = zone;
}

WalletHeaderSyncWorker::~WalletHeaderSyncWorker() {
	delete this->nodeId;
	this->parent = nullptr;
}


void WalletHeaderSyncWorker::doProcess() {
	WalletNetworkManager* networkManager = this->parent->getWalletNetworkManager();

	const NodeIdentifierSource* source = networkManager->getNodeIdentifierSource();

	syncHeader(networkManager, source, this->startHeight);
}

void WalletHeaderSyncWorker::syncHeader(WalletNetworkManager *networkManager, const NodeIdentifierSource *source, uint64_t startHeight) {
	uint64_t offset = startHeight;
	int limit = 100;
	bool hasnext = true;

	while(hasnext){
		ClientSyncHeaderCommand cmd;
		cmd.setZone(this->zone);
		cmd.setStartHeight(offset);
		cmd.setNumRequestHeight(limit);

		cmd.sign(source);

		ClientNodeHandshake* handshake = networkManager->getClientHandshakeByNodeId(this->nodeId);
		ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(handshake == nullptr, L"Node connection has alrealy closed.", __FILE__, __LINE__);

		StackHandshakeReleaser __releaser(handshake);

		AbstractCommandResponse* response = handshake->sendCommnad(&cmd); __STP(response);
		ClientSyncHeaderCommandResponse* syncResponse = dynamic_cast<ClientSyncHeaderCommandResponse*>(response);

		ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(syncResponse == nullptr, L"Respose error.", __FILE__, __LINE__);

		{
			UnicodeString message(L"Accept Response : ");
			UnicodeString* responseStr = response->toString(); __STP(responseStr);
			message.append(responseStr);

			this->logger->debugLog(ISystemLogger::DEBUG_NODE_TRANSFER_RESPONSE, &message, __FILE__, __LINE__);
		}

		int nread = syncResponse->getDataCount();
		hasnext = (limit == nread);

		for(int i = 0; i != nread; ++i){
			const ClientSyncHeaderHeightData* headerData = syncResponse->get(i);

			// verify Merkle Certification
			headerData->verifyVoteTransactionsData();

			// verify transactions
			headerData->verifyTransactionsData();

			// store into repo
			this->parent->addHeaderData(headerData);
		}

		offset += nread;
	}

}

} /* namespace codablecash */
