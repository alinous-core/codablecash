/*
 * NetworkClientCommandProcessor.cpp
 *
 *  Created on: Mar 21, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net/NetworkClientCommandProcessor.h"

#include "bc_p2p_cmd_client_notify/ClientExecutor.h"


namespace codablecash {

NetworkClientCommandProcessor::NetworkClientCommandProcessor(NetworkWallet* networkWallet) {
	this->networkWallet = networkWallet;
	this->clientExec = new ClientExecutor();
}

NetworkClientCommandProcessor::~NetworkClientCommandProcessor() {
	this->networkWallet = nullptr;
	delete this->clientExec;
}

void NetworkClientCommandProcessor::fireExecuteCommand(const AbstractPubSubCommand *cmd) {
}

void NetworkClientCommandProcessor::fireOnExeptionOnSubscriber(const Exception *e) {
}

void NetworkClientCommandProcessor::fireOnSubscribeEnds() {
}

IPubsubCommandExecutor* NetworkClientCommandProcessor::getExecutor() const noexcept {
	return this->clientExec;
}

} /* namespace codablecash */
