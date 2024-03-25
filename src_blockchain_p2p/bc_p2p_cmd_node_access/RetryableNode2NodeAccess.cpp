/*
 * RetryableNode2NodeAccess.cpp
 *
 *  Created on: 2023/11/19
 *      Author: iizuka
 */

#include <cstdlib>

#include "bc_p2p_cmd_node_access/RetryableNode2NodeAccess.h"
#include "bc_p2p_cmd_node_access/IRetryableResponceValidator.h"
#include "bc_p2p_cmd_node_access/RetryableNode2NodeAccessException.h"

#include "base/Exception.h"
#include "base/StackRelease.h"

#include "bc_p2p/StackHandshakeReleaser.h"
#include "bc_p2p/BlockchainNodeHandshake.h"
#include "bc_p2p/BlochchainP2pManager.h"

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "bc/ISystemLogger.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_p2p_cmd_node/AbstractNodeCommand.h"

namespace codablecash {

RetryableNode2NodeAccess::RetryableNode2NodeAccess(uint16_t zone, AbstractNodeCommand* command, int retryTimes, BlochchainP2pManager* p2pManager
		, IRetryableResponceValidator* validator, ISystemLogger* logger) {
	this->zone = zone;
	this->command = dynamic_cast<AbstractNodeCommand*>(command->copyData());
	this->retryTimes = retryTimes;
	this->p2pManager = p2pManager;
	this->validator = validator;
	this->logger = logger;
}

RetryableNode2NodeAccess::~RetryableNode2NodeAccess() {
	delete this->command;
}

AbstractCommandResponse* RetryableNode2NodeAccess::access() {
	ArrayList<NodeIdentifier>* nodelist = this->p2pManager->getNodeIds(this->zone); __STP(nodelist);
	nodelist->setDeleteOnExit();
	int nodelistSize = nodelist->size();

	int index = ::rand();

	for(int i = 0; i != this->retryTimes; ++i){
		int pos = (index++) % nodelistSize;
		NodeIdentifier* nodeId = nodelist->get(pos);

		try{
			BlockchainNodeHandshake* handshake = this->p2pManager->getNodeHandshakeByNodeId(nodeId);
			StackHandshakeReleaser __releaser(handshake);

			AbstractCommandResponse* response = handshake->sendCommnad(this->command); __STP(response);
			if(this->validator->validate(response)){
				return __STP_MV(response);
			}
		}
		catch(Exception* e){
			__STP(e);
		}
	}

	throw new RetryableNode2NodeAccessException(__FILE__, __LINE__);
}

} /* namespace codablecash */
