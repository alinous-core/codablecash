/*
 * HeaderSyncWorker.cpp
 *
 *  Created on: 2023/11/05
 *      Author: iizuka
 */

#include "bc_network_instance_sync/HeaderSyncWorker.h"
#include "bc_network_instance_sync/AbstractZoneSync.h"

#include "bc_network_instance/CodablecashNetworkNode.h"

#include "bc_network/NodeIdentifier.h"
#include "bc_network/NodeIdentifierSource.h"

#include "bc/CodablecashNodeInstance.h"
#include "bc/CodablecashConfig.h"
#include "bc/ExceptionThrower.h"

#include "bc_p2p/BlockchainNodeHandshake.h"
#include "bc_p2p/BlochchainP2pManager.h"
#include "bc_p2p/BlockchainNodeHandshakeException.h"
#include "bc_p2p/StackHandshakeReleaser.h"

#include "bc_p2p_cmd_node/SyncHeaderNodeCommand.h"
#include "bc_p2p_cmd_node/SyncHeaderNodeCommandResponse.h"
#include "bc_p2p_cmd_node/SyncHeaderHeightData.h"

#include "base/StackRelease.h"

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc/ISystemLogger.h"


namespace codablecash {

HeaderSyncWorker::HeaderSyncWorker(uint16_t zone, const NodeIdentifier* nodeId, AbstractZoneSync* parent, ISystemLogger* logger, const UnicodeString* name)
		: AbstractSyncWorkerThread(logger, name){
	this->nodeId = new NodeIdentifier(*nodeId);
	this->parent = parent;
	this->zone = zone;
}

HeaderSyncWorker::~HeaderSyncWorker() {
	delete this->nodeId;
	this->parent = nullptr;
}

void HeaderSyncWorker::doProcess() {
	CodablecashNetworkNode* node = this->parent->getCodablecashNetworkNode();
	CodablecashNodeInstance* inst = node->getInstance();
	CodablecashConfig* config = inst->getCodablecashConfig();

	BlochchainP2pManager* p2pManager = inst->getBlochchainP2pManager();

	const NodeIdentifierSource *source = inst->getNetworkKey();

	uint64_t finalizedHeight = 0;
	{
		BlockchainController* ctrl = inst->getController();
		finalizedHeight = ctrl->getFinalizedHeight(inst->getZoneSelf());

		finalizedHeight = finalizedHeight + 1;
	}

	syncHeader(p2pManager, source, finalizedHeight);
}

void HeaderSyncWorker::syncHeader(BlochchainP2pManager *p2pManager, const NodeIdentifierSource *source, uint64_t startHeight) {

	uint64_t offset = startHeight;
	int limit = 100;
	bool hasnext = true;

	while(hasnext){
		SyncHeaderNodeCommand cmd;
		cmd.setZone(this->zone);
		cmd.setStartHeight(offset);
		cmd.setNumRequestHeight(limit);

		cmd.sign(source);

		BlockchainNodeHandshake* handshake = p2pManager->getNodeHandshakeByNodeId(this->nodeId);
		ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(handshake == nullptr, L"Node connection has alrealy closed.", __FILE__, __LINE__);

		StackHandshakeReleaser __releaser(handshake);

		AbstractCommandResponse* response = handshake->sendCommnad(&cmd); __STP(response);
		SyncHeaderNodeCommandResponse* syncResponse = dynamic_cast<SyncHeaderNodeCommandResponse*>(response);

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
			const SyncHeaderHeightData* headerData = syncResponse->get(i);

			// verify Merkle Certification
			headerData->verifyVoteTransactionsData();

			// store into repo
			this->parent->addHeaderData(headerData);
		}

		offset += nread;
	}
}

} /* namespace codablecash */
