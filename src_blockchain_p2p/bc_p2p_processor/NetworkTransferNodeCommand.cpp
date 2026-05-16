/*
 * NetworkTransferCommand.cpp
 *
 *  Created on: 2023/09/26
 *      Author: iizuka
 */

#include "bc_p2p_processor/P2pRequestProcessor.h"

#include "bc_p2p_cmd_node/AbstractNodeCommand.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_p2p/BlochchainP2pManager.h"
#include "bc_p2p/StackHandshakeReleaser.h"
#include "bc_p2p/BlockchainNodeHandshake.h"
#include "bc_p2p/BlockchainNodeHandshakeException.h"

#include "bc_p2p_processor/NetworkTransferNodeCommand.h"

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "base/StackRelease.h"

#include "bc/ISystemLogger.h"

#include "bc/ExceptionThrower.h"


namespace codablecash {

NetworkTransferNodeCommand::NetworkTransferNodeCommand(const NodeIdentifier *nodeId, const AbstractNodeCommand *command)
		: AbstractNetworkTransferCommandMessage() {
	this->nodeId = dynamic_cast<NodeIdentifier*>(nodeId->copyData());
	this->command = dynamic_cast<AbstractNodeCommand*>(command->copyData());
}

NetworkTransferNodeCommand::~NetworkTransferNodeCommand() {
	delete this->nodeId;
	delete this->command;
}

void NetworkTransferNodeCommand::execute(ICommandParameter *param) {
	P2pRequestProcessor* processor = dynamic_cast<P2pRequestProcessor*>(param);
	ISystemLogger* logger = processor->getLogger();

	BlochchainP2pManager* p2pManager = processor->getBlochchainP2pManager();

	BlockchainNodeHandshake* handshake = p2pManager->getNodeHandshakeByNodeId(this->nodeId);
	StackHandshakeReleaser __releaser(handshake);

	// exception handshake
	ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(handshake == nullptr, L"Node connection has alrealy closed.", __FILE__, __LINE__);

	AbstractCommandResponse* response = handshake->sendCommnad(this->command); __STP(response);
	if(response != nullptr){
		UnicodeString* message = response->toString(); __STP(message);

		logger->debugLog(ISystemLogger::DEBUG_POOL_TRANSFER_RESPONSE, message, __FILE__, __LINE__);
	}
}

} /* namespace codablecash */
