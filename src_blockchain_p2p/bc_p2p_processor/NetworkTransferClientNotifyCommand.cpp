/*
 * NetworkTransferClientNotifyCommand.cpp
 *
 *  Created on: 2023/10/07
 *      Author: iizuka
 */

#include "bc_p2p_processor/NetworkTransferClientNotifyCommand.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_p2p_cmd_client_notify/AbstractClientNotifyCommand.h"

#include "bc_p2p_processor/P2pRequestProcessor.h"

#include "bc_p2p/BlockchainNodeHandshake.h"
#include "bc_p2p/BlochchainP2pManager.h"
#include "bc_p2p/ClientNodeHandshake.h"
#include "bc_p2p/StackHandshakeReleaser.h"

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "base/StackRelease.h"

#include "bc/ISystemLogger.h"

namespace codablecash {

NetworkTransferClientNotifyCommand::NetworkTransferClientNotifyCommand(const NodeIdentifier *nodeId, const AbstractClientNotifyCommand *command) {
	this->nodeId = dynamic_cast<NodeIdentifier*>(nodeId->copyData());
	this->command = dynamic_cast<AbstractClientNotifyCommand*>(command->copyData());
}

NetworkTransferClientNotifyCommand::~NetworkTransferClientNotifyCommand() {
	delete this->nodeId;
	delete this->command;
}

void NetworkTransferClientNotifyCommand::execute(ICommandParameter *param) {
	P2pRequestProcessor* processor = dynamic_cast<P2pRequestProcessor*>(param);
	ISystemLogger* logger = processor->getLogger();

	BlochchainP2pManager* p2pManager = processor->getBlochchainP2pManager();

	ClientNodeHandshake* handshake = p2pManager->getClientHandshakeByNodeId(this->nodeId);
	StackHandshakeReleaser __releaser(handshake);

	AbstractCommandResponse* response = handshake->sendCommnad(this->command); __STP(response);
	if(response != nullptr){
		UnicodeString* message = response->toString(); __STP(message);

		logger->debugLog(ISystemLogger::DEBUG_NODE_TRANSFER_RESPONSE, message, __FILE__, __LINE__);
	}
}

} /* namespace codablecash */
