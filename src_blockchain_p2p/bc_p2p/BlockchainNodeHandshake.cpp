/*
 * BlockchainNodeHandshake.cpp
 *
 *  Created on: 2023/04/05
 *      Author: iizuka
 */

#include "bc_p2p/BlockchainNodeHandshake.h"

#include "pubsub/P2pHandshake.h"

#include "bc_network/NodeIdentifier.h"

#include "base_thread/SysMutex.h"
#include "base_thread/StackUnlocker.h"

#include "bc_p2p_cmd_node/AbstractNodeCommand.h"

#include "bc_p2p_cmd_client_notify/AbstractClientNotifyCommand.h"

namespace codablecash {

BlockchainNodeHandshake::BlockchainNodeHandshake(P2pHandshake *handshake, int zone, const NodeIdentifier* nodeId) {
	this->nodeId = dynamic_cast<NodeIdentifier*>(nodeId->copyData());
	this->zone = zone;
	handshake->addRef();
	this->handshake = handshake;

	this->mutex = new SysMutex();
	this->ref = 0;
}

BlockchainNodeHandshake::~BlockchainNodeHandshake() {
	dispose(true);

	delete this->nodeId;
	delete this->mutex;
}

void BlockchainNodeHandshake::dispose(bool force) noexcept {
	if(this->handshake != nullptr){
		this->handshake->decRef();
		this->handshake->setDisabled(true);

		this->handshake = nullptr;
	}
}

const PubSubId* BlockchainNodeHandshake::getPubsubId() const noexcept {
	return this->handshake->getPubsubid();
}

void BlockchainNodeHandshake::incRef() noexcept {
	StackUnlocker __lock(this->mutex);
	this->ref++;
}

void BlockchainNodeHandshake::decRef() noexcept {
	StackUnlocker __lock(this->mutex);
	this->ref--;
}

bool BlockchainNodeHandshake::isDeletable() const noexcept {
	StackUnlocker __lock(this->mutex);
	return this->ref == 0;
}

AbstractCommandResponse* BlockchainNodeHandshake::sendCommnad(const AbstractNodeCommand *command) {
	StackUnlocker __lock(this->mutex);

	return this->handshake->publishCommand(command);
}

AbstractCommandResponse* BlockchainNodeHandshake::sendCommnad(const AbstractClientNotifyCommand *command) {
	StackUnlocker __lock(this->mutex);

	return this->handshake->publishCommand(command);
}

} /* namespace codablecash */
