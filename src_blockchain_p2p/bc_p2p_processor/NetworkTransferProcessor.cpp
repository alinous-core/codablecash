/*
 * NetworkTransferProcessor.cpp
 *
 *  Created on: 2023/09/05
 *      Author: iizuka
 */

#include "bc_p2p_processor/NetworkTransferProcessor.h"

#include "procesor_multi/MultipleCommandProcessor.h"

#include "bc_p2p_processor/P2pRequestProcessor.h"
#include "bc_p2p_processor/NetworkTransferNodeCommand.h"
#include "bc_p2p_processor/NetworkTransferClientNotifyCommand.h"

#include "bc/ISystemLogger.h"

namespace codablecash {

NetworkTransferProcessor::NetworkTransferProcessor(P2pRequestProcessor* processor, int numThreads, ISystemLogger* logger) {
	this->processor = processor;
	this->numThreads = numThreads;
	this->logger = logger;

	this->executor = nullptr;

	this->serial = 0;
}

NetworkTransferProcessor::~NetworkTransferProcessor() {
	shutdown();
}

void NetworkTransferProcessor::start() {
	if(this->executor == nullptr){
		this->executor = new MultipleCommandProcessor(this->processor, this->numThreads, this->logger, THREAD_NAME);
		this->executor->start();
	}
}

void NetworkTransferProcessor::shutdown() {
	if(this->executor != nullptr){
		this->executor->shutdown();
		delete this->executor;
		this->executor = nullptr;
	}
}

void NetworkTransferProcessor::reserveTransfer(const NodeIdentifier *nodeId, const AbstractNodeCommand *command) {
	NetworkTransferNodeCommand* messageCommnad = new NetworkTransferNodeCommand(nodeId, command);

	int num = this->serial % this->executor->size();
	this->serial++;

	this->executor->addCommandMessage(messageCommnad, num);
}

void NetworkTransferProcessor::reserveClientNotifyTransfer(const NodeIdentifier *nodeId, const AbstractClientNotifyCommand *command) {
	NetworkTransferClientNotifyCommand* messageCommnad = new NetworkTransferClientNotifyCommand(nodeId, command);

	int num = this->serial % this->executor->size();
	this->serial++;

	this->executor->addCommandMessage(messageCommnad, num);
}

} /* namespace codablecash */
