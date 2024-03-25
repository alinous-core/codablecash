/*
 * BlochchainP2pManager.cpp
 *
 *  Created on: 2023/04/05
 *      Author: iizuka
 */

#include "bc_p2p/BlochchainP2pManager.h"
#include "bc_p2p/BlockchainZoneException.h"
#include "bc_p2p/P2pZone.h"

#include "pubsub/PubSubId.h"
#include "pubsub/P2pHandshake.h"

#include "bc_p2p_cmd/LoginPubSubCommand.h"
#include "bc_p2p_cmd/LoginClientCommand.h"

#include "bc/ExceptionThrower.h"

#include "bc_p2p/ClientNodeHandshake.h"

#include "base/StackRelease.h"

#include "bc_network/NodeIdentifier.h"

#include "base_thread/SysMutex.h"
#include "base_thread/StackUnlocker.h"

#include "debug/debugMacros.h"

#include "osenv/funcs.h"

using alinous::Os;
namespace codablecash {

BlochchainP2pManager::BlochchainP2pManager() {
	this->mutex = new SysMutex();
	this->zones = new ArrayList<P2pZone>();
	this->end = false;
}

BlochchainP2pManager::~BlochchainP2pManager() {
	resetZones();
	delete this->zones;
	delete this->mutex;
}

void BlochchainP2pManager::endCommunication() noexcept {
	StackUnlocker __lock(this->mutex);

	this->end = true;
}

void BlochchainP2pManager::resetZones() noexcept {
	// move to remove
	{
		StackUnlocker __lock(this->mutex);

		{
			ArrayList<PubSubId> list;
			list.setDeleteOnExit();

			Iterator<PubSubId>* it = this->blockchainHandshakeHash.keySet()->iterator(); __STP(it);
			while(it->hasNext()){
				const PubSubId* pubsubId = it->next();

				list.addElement(new PubSubId(*pubsubId));
			}

			int maxLoop = list.size();
			for(int i = 0; i != maxLoop; ++i){
				PubSubId* pubsubId = list.get(i);
				__removeHandshake(pubsubId);
			}
		}

		{
			ArrayList<PubSubId> list;
			list.setDeleteOnExit();

			Iterator<PubSubId>* it = this->clientHandshakeHash.keySet()->iterator(); __STP(it);
			while(it->hasNext()){
				const PubSubId* pubsubId = it->next();

				list.addElement(new PubSubId(*pubsubId));
			}

			int maxLoop = list.size();
			for(int i = 0; i != maxLoop; ++i){
				PubSubId* pubsubId = list.get(i);
				__removeHandshake(pubsubId);
			}
		}
	}

	// wait for removed
	bool removed = false;
	while(!removed){
		{
			StackUnlocker __lock(this->mutex);

			int deletedZoneCount = 0;
			int maxLoop = this->zones->size();
			for(int i = 0; i != maxLoop; ++i){
				P2pZone* zone = this->zones->get(i);

				if(zone->checkRemovalStatus()){
					deletedZoneCount++;
				}
			}

			removed = (deletedZoneCount == maxLoop);
		}

		Os::usleep(100);
	}

	// actually delete zones
	{
		StackUnlocker __lock(this->mutex);

		this->zones->deleteElements();
		this->zones->reset();

		this->blockchainHandshakeHash.clear();
		this->clientHandshakeHash.clear();
	}
}

void BlochchainP2pManager::__removeHandshake(const PubSubId *pubsubId) {
	int maxLoop = this->zones->size();
	for(int i = 0; i != maxLoop; ++i){
		P2pZone* z = this->zones->get(i);
		z->remove(pubsubId);
	}

	this->blockchainHandshakeHash.remove(pubsubId);
	this->clientHandshakeHash.remove(pubsubId);
}

void BlochchainP2pManager::init(int numZones) {
	StackUnlocker __lock(this->mutex);

	for(int i = 0; i != numZones; ++i){
		P2pZone* z = new P2pZone(i);
		this->zones->addElement(z);
	}
}

void BlochchainP2pManager::onLoginHandshake(P2pHandshake *handshake, const LoginPubSubCommand *loginCommand) {
	StackUnlocker __lock(this->mutex);

	uint16_t zone = loginCommand->getZone();
	const NodeIdentifier* nodeId = loginCommand->getNodeId();

	ExceptionThrower<BlockchainZoneException>::throwExceptionIfCondition(zone > this->zones->size(), L"Zone does not exists", __FILE__, __LINE__);

	P2pZone* z = this->zones->get(zone);

	BlockchainNodeHandshake* blockchainHandshake = z->add(handshake, nodeId);

	const PubSubId* pubsubId = handshake->getPubsubid();
	this->blockchainHandshakeHash.put(pubsubId, blockchainHandshake);
}

void BlochchainP2pManager::registerHandshake(uint16_t zone, P2pHandshake *handshake, const NodeIdentifier *nodeId) {
	CAUSE_ERROR_BY_THROW(L"BlochchainP2pManager::registerHandshake", new BlockchainZoneException(__FILE__, __LINE__));

	StackUnlocker __lock(this->mutex);

	ExceptionThrower<BlockchainZoneException>::throwExceptionIfCondition(zone > this->zones->size(), L"Zone does not exists", __FILE__, __LINE__);

	P2pZone* z = this->zones->get(zone);

	BlockchainNodeHandshake* blockchainHandshake = z->add(handshake, nodeId);

	const PubSubId* pubsubId = handshake->getPubsubid();
	this->blockchainHandshakeHash.put(pubsubId, blockchainHandshake);
}

void BlochchainP2pManager::onLoginClinentHandshake(P2pHandshake *handshake, const LoginClientCommand *clientLoginCommand) {
	StackUnlocker __lock(this->mutex);

	uint16_t zone = clientLoginCommand->getZone();
	const NodeIdentifier* nodeId = clientLoginCommand->getNodeId();

	ExceptionThrower<BlockchainZoneException>::throwExceptionIfCondition(zone > this->zones->size(), L"Zone does not exists", __FILE__, __LINE__);

	P2pZone* z = this->zones->get(zone);
	ClientNodeHandshake* clientHandshake = z->addClient(handshake, nodeId);

	const PubSubId* pubsubId = handshake->getPubsubid();
	this->clientHandshakeHash.put(pubsubId, clientHandshake);
}

void BlochchainP2pManager::onHandshakeEnds(P2pHandshake *handshake) {
	StackUnlocker __lock(this->mutex);

	const PubSubId* pubsubId = handshake->getPubsubid();

	int maxLoop = this->zones->size();
	for(int i = 0; i != maxLoop; ++i){
		P2pZone* z = this->zones->get(i);
		z->remove(pubsubId);
	}

	this->blockchainHandshakeHash.remove(pubsubId);
	this->clientHandshakeHash.remove(pubsubId);
}

BlockchainNodeHandshake* BlochchainP2pManager::getNodeHandshake(const PubSubId *pubsubId) const noexcept {
	StackUnlocker __lock(this->mutex);

	BlockchainNodeHandshake* ret = this->blockchainHandshakeHash.get(pubsubId);
	if(ret != nullptr){
		ret->incRef();
	}

	return ret;
}

ClientNodeHandshake* BlochchainP2pManager::getClientHandshake(const PubSubId *pubsubId) const noexcept {
	StackUnlocker __lock(this->mutex);
	return this->clientHandshakeHash.get(pubsubId);
}

BlockchainNodeHandshake* BlochchainP2pManager::getNodeHandshakeByNodeId(const NodeIdentifier *nodeId) const noexcept {
	StackUnlocker __lock(this->mutex);

	BlockchainNodeHandshake* ret = nullptr;

	Iterator<PubSubId>* it = this->blockchainHandshakeHash.keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const PubSubId* key = it->next();
		BlockchainNodeHandshake* handshake = this->blockchainHandshakeHash.get(key);

		const NodeIdentifier* id = handshake->getNodeId();
		if(nodeId->compareTo(id) == 0){
			ret = handshake;
			ret->incRef();
			break;
		}
	}

	return ret;
}

ClientNodeHandshake* BlochchainP2pManager::getClientHandshakeByNodeId(const NodeIdentifier *nodeId) const noexcept {
	StackUnlocker __lock(this->mutex);

	ClientNodeHandshake* ret = nullptr;

	Iterator<PubSubId>* it = this->clientHandshakeHash.keySet()->iterator(); __STP(it);

	while(it->hasNext()){
		const PubSubId* key = it->next();
		ClientNodeHandshake* handshake = this->clientHandshakeHash.get(key);

		const NodeIdentifier* id = handshake->getNodeId();
		if(nodeId->compareTo(id) == 0){
			ret = handshake;
			ret->incRef();
			break;
		}
	}

	return ret;
}

void BlochchainP2pManager::bloadCastWithinZone(uint16_t zoneSelf,	const AbstractNodeCommand *command, P2pRequestProcessor* processor) {
	StackUnlocker __lock(this->mutex);

	if(!this->end){
		__bloadCastWithinZone(zoneSelf, nullptr, command, processor);
	}
}

void BlochchainP2pManager::bloadCastWithinZone(uint16_t zoneSelf, const NodeIdentifier *excludeNodeId, const AbstractNodeCommand *command, P2pRequestProcessor *processor) {
	StackUnlocker __lock(this->mutex);

	if(!this->end){
		__bloadCastWithinZone(zoneSelf, excludeNodeId, command, processor);
	}
}

void BlochchainP2pManager::bloadCastToClients(AbstractClientNotifyCommand *commnad, P2pRequestProcessor *processor) {
	StackUnlocker __lock(this->mutex);

	if(!this->end){
		int maxLoop = this->zones->size();
		for(int i = 0; i != maxLoop; ++i){
			P2pZone* zone = this->zones->get(i);

			zone->generateClientNotifyRequest(commnad, processor);
		}
	}
}

void BlochchainP2pManager::bloadCastAllZones(const NodeIdentifier *excludeNodeId, const AbstractNodeCommand *command, P2pRequestProcessor *processor) {
	StackUnlocker __lock(this->mutex);

	if(!this->end){
		int maxLoop = this->zones->size();
		for(int i = 0; i != maxLoop; ++i){
			__bloadCastWithinZone(i, excludeNodeId, command, processor);
		}
	}

}

void BlochchainP2pManager::__bloadCastWithinZone(uint16_t zone, const NodeIdentifier *excludeNodeId, const AbstractNodeCommand *command,
		P2pRequestProcessor *processor) {
	if(!this->end){
		P2pZone* p2pzone = this->zones->get(zone);

		p2pzone->generateTransferRequest(excludeNodeId, command, processor);
	}
}

int BlochchainP2pManager::getNumNodes(uint16_t zone) const {
	StackUnlocker __lock(this->mutex);

	P2pZone* p2pZone = this->zones->get(zone);

	return p2pZone->nodeSize();
}

ArrayList<NodeIdentifier>* BlochchainP2pManager::getNodeIds(uint16_t zone) const {
	StackUnlocker __lock(this->mutex);

	ArrayList<NodeIdentifier>* list = new ArrayList<NodeIdentifier>(); __STP(list);
	list->setDeleteOnExit();

	P2pZone* p2pZone = this->zones->get(zone);
	p2pZone->exportNodesIdList(list);

	list->setDeleteOnExit(false);
	return __STP_MV(list);
}

} /* namespace codablecash */
