/*
 * RetriableAccessContainer.cpp
 *
 *  Created on: Aug 10, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_access/RetriableClientAccessContainer.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_wallet_net/WalletNetworkManager.h"

#include "base/ArrayList.h"
#include "base/StackRelease.h"
#include "base/Exception.h"

#include "bc_network_instance_sync/RamdomNodesSelector.h"

#include "bc_p2p/StackHandshakeReleaser.h"
#include "bc_p2p/ClientNodeHandshake.h"

#include "bc/ISystemLogger.h"

#include "bc_wallet_net_access/IRetriableClientAccess.h"

#include "bc_p2p_cmd_node_access/RetryableNode2NodeAccessException.h"

using namespace alinous;

namespace codablecash {

RetriableClientAccessContainer::RetriableClientAccessContainer(IRetriableClientAccess* access, ISystemLogger* logger, WalletNetworkManager* networkManager) {
	this->access = access;
	this->logger = logger;
	this->networkManager = networkManager;
}

RetriableClientAccessContainer::~RetriableClientAccessContainer() {
	this->access = nullptr;
	this->logger = nullptr;
	this->networkManager = nullptr;
}

void RetriableClientAccessContainer::clientAccess() {
	ArrayList<NodeIdentifier>* nodelist = this->networkManager->getNodeIdList(); __STP(nodelist);
	nodelist->setDeleteOnExit();

	int num = nodelist->size();
	RamdomNodesSelector sel(nodelist, num);

	while(sel.hasNext()){
		const NodeIdentifier* nodeId = sel.next();

		ClientNodeHandshake* handshake = this->networkManager->getClientHandshakeByNodeId(nodeId);
		StackHandshakeReleaser __releaser(handshake);

		try{
			this->access->access(handshake, this->logger);
			return;
		}
		catch(Exception* e){
			this->logger->logException(e);
		}
	}

	throw new RetryableNode2NodeAccessException(__FILE__, __LINE__);
}

} /* namespace codablecash */
