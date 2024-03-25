/*
 * P2pZone.cpp
 *
 *  Created on: 2023/04/05
 *      Author: iizuka
 */

#include "bc_p2p/P2pZone.h"
#include "bc_p2p/BlockchainNodeHandshake.h"
#include "bc_p2p/BlockchainZoneException.h"
#include "bc_p2p/ClientNodeHandshake.h"

#include "base_thread/SysMutex.h"
#include "base_thread/StackUnlocker.h"

#include "pubsub/P2pHandshake.h"
#include "pubsub/PubSubId.h"

#include "bc/ExceptionThrower.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_p2p_processor/NetworkTransferProcessor.h"
#include "bc_p2p_processor/P2pRequestProcessor.h"

#include "base/RawArrayPrimitive.h"

#include "osenv/funcs.h"

using alinous::Os;

namespace codablecash {

P2pZone::P2pZone(uint16_t zone) {
	this->zone = zone;
	this->mutex = new SysMutex();
}

P2pZone::~P2pZone() {
	assert(this->nodeslist.isEmpty());
	assert(this->clientslist.isEmpty());
	assert(this->removeList.isEmpty());

	delete this->mutex;
}

BlockchainNodeHandshake* P2pZone::add(P2pHandshake *handshake, const NodeIdentifier* nodeId) {
	StackUnlocker __lock(this->mutex);

	// check
	checkAlreadyRegistered(handshake->getPubsubid());

	BlockchainNodeHandshake* nodeHandshake = new BlockchainNodeHandshake(handshake, this->zone, nodeId);
	this->nodeslist.addElement(nodeHandshake);

	__removeRemovable();

	return nodeHandshake;
}

ClientNodeHandshake* P2pZone::addClient(P2pHandshake *handshake, const NodeIdentifier* nodeId) {
	StackUnlocker __lock(this->mutex);

	// check
	checkAlreadyRegistered(handshake->getPubsubid());

	ClientNodeHandshake* nodeHandshake = new ClientNodeHandshake(handshake, this->zone, nodeId);
	this->clientslist.addElement(nodeHandshake);

	__removeRemovable();

	return nodeHandshake;
}

void P2pZone::remove(const PubSubId *pubsubId) noexcept {
	StackUnlocker __lock(this->mutex);

	BlockchainNodeHandshake* removeNode = __getHandshake(pubsubId, &this->nodeslist);

	if(removeNode != nullptr){
		this->nodeslist.removeByObj(removeNode);
		this->removeList.addElement(removeNode);
	}

	ClientNodeHandshake* removedClientNode = __getClientHandshake(pubsubId, &this->clientslist);
	if(removedClientNode != nullptr){
		this->clientslist.removeByObj(removedClientNode);
		this->removeList.addElement(removedClientNode);
	}
}

bool P2pZone::checkRemovalStatus() {
	StackUnlocker __lock(this->mutex);

	__removeRemovable();

	return this->removeList.isEmpty();
}


void P2pZone::checkAlreadyRegistered(const PubSubId *pubsubId) {
	// check
	{
		BlockchainNodeHandshake* habndshake = __getHandshake(pubsubId, &this->nodeslist);
		ExceptionThrower<BlockchainZoneException>::throwExceptionIfCondition(habndshake != nullptr, L"The zone has already the PubsubId.", __FILE__, __LINE__);

		ClientNodeHandshake* clientNode  = __getClientHandshake(pubsubId, &this->clientslist);
		ExceptionThrower<BlockchainZoneException>::throwExceptionIfCondition(clientNode != nullptr, L"The zone has already the PubsubId.", __FILE__, __LINE__);
	}
}


BlockchainNodeHandshake* P2pZone::__getHandshake(const PubSubId *pubsubid, ArrayList<BlockchainNodeHandshake>* list) noexcept {
	BlockchainNodeHandshake* ret = nullptr;

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		BlockchainNodeHandshake* inst = list->get(i);
		const PubSubId* psid = inst->getPubsubId();

		if(psid->equals(pubsubid)){
			ret = inst;
			break;
		}
	}

	return ret;
}

ClientNodeHandshake* P2pZone::__getClientHandshake(const PubSubId *pubsubid, ArrayList<ClientNodeHandshake> *list) noexcept {
	ClientNodeHandshake* ret = nullptr;

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		ClientNodeHandshake* inst = list->get(i);
		const PubSubId* psid = inst->getPubsubId();

		if(psid->equals(pubsubid)){
			ret = inst;
			break;
		}
	}

	return ret;
}

void P2pZone::generateTransferRequest(const NodeIdentifier *excludeNodeId, const AbstractNodeCommand *command, P2pRequestProcessor *processor) {
	StackUnlocker __lock(this->mutex);

	NetworkTransferProcessor* nodeTransferProcessor = processor->getNodeTransferProcessor();

	int maxLoop = this->nodeslist.size();
	for(int i = 0; i != maxLoop; ++i){
		BlockchainNodeHandshake* nodeHandshake = this->nodeslist.get(i);
		const NodeIdentifier* nodeId = nodeHandshake->getNodeId();

		if(excludeNodeId != nullptr && excludeNodeId->compareTo(nodeId) == 0){
			continue;
		}

		nodeTransferProcessor->reserveTransfer(nodeId, command);
	}
}

void P2pZone::generateClientNotifyRequest(AbstractClientNotifyCommand *commnad, P2pRequestProcessor *processor) {
	StackUnlocker __lock(this->mutex);

	NetworkTransferProcessor* nodeTransferProcessor = processor->getNodeTransferProcessor();

	int maxLoop = this->clientslist.size();
	for(int i = 0; i != maxLoop; ++i){
		ClientNodeHandshake* client = this->clientslist.get(i);
		const NodeIdentifier* nodeId = client->getNodeId();

		nodeTransferProcessor->reserveClientNotifyTransfer(nodeId, commnad);
	}
}

void P2pZone::__removeRemovable() noexcept {
	RawArrayPrimitive<int> delList(64);

	{
		int maxLoop = this->removeList.size();
		for(int i = 0; i != maxLoop; ++i){
			BlockchainNodeHandshake* handshake = this->removeList.get(i);

			if(handshake->isDeletable()){
				handshake->dispose(true);

				delete handshake;
				delList.addElement(i);
			}
		}
	}

	{
		int top = delList.size() - 1;
		for(int i = top; i >= 0; --i){
			int index = delList.get(i);

			this->removeList.remove(index);
		}
	}
}

int P2pZone::nodeSize() const noexcept {
	StackUnlocker __lock(this->mutex);

	return this->nodeslist.size();
}

void P2pZone::exportNodesIdList(ArrayList<NodeIdentifier> *list) noexcept {
	StackUnlocker __lock(this->mutex);

	int maxLoop = this->nodeslist.size();
	for(int i = 0; i != maxLoop; ++i){
		BlockchainNodeHandshake* handshake = this->nodeslist.get(i);
		const NodeIdentifier* nodeId = handshake->getNodeId();

		list->addElement(new NodeIdentifier(*nodeId));
	}
}

} /* namespace codablecash */
