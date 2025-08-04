/*
 * WalletConnectionManager.cpp
 *
 *  Created on: Feb 21, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net/WalletConnectionManager.h"

#include "bc_wallet_net_processor/NetworkClientCommandProcessor.h"

#include "bc_network/NodeIdentifierSource.h"

#include "pubsub/PubSubId.h"
#include "pubsub/P2pHandshake.h"

#include "pubsub_cmd/AbstractCommandResponse.h"
#include "pubsub_cmd/OkPubsubResponse.h"

#include "bc_p2p/ClientNodeHandshake.h"

#include "base_thread/SysMutex.h"
#include "base_thread/StackUnlocker.h"

#include "base/StackRelease.h"
#include "base/Exception.h"

#include "osenv/funcs.h"

#include "bc/ISystemLogger.h"

#include "bc_p2p_cmd/LoginClientCommand.h"

#include "bc/ExceptionThrower.h"

#include "bc_p2p_client/P2pClientConnectionException.h"

#include "bc_network/NodeIdentifier.h"

namespace codablecash {

WalletConnectionManager::WalletConnectionManager(uint16_t defaultZone, NetworkClientCommandProcessor* clientCommandProcessor) {
	this->mutex = new SysMutex();
	this->defaultZone = defaultZone;

	this->source = NodeIdentifierSource::create();
	this->clientCommandProcessor = clientCommandProcessor;
}

WalletConnectionManager::~WalletConnectionManager() {
	dispose();

	delete this->source;
	delete this->mutex;
	this->clientCommandProcessor = nullptr;
}

void WalletConnectionManager::dispose() {
	// move into disposal list
	{
		StackUnlocker __unlock(this->mutex, __FILE__, __LINE__);

		ArrayList<PubSubId> dellist;
		dellist.setDeleteOnExit();

		Iterator<PubSubId>* it = this->clientHandshakeHash.keySet()->iterator(); __STP(it);
		while(it->hasNext()){
			const PubSubId* key = it->next();

			dellist.addElement(new PubSubId(*key));
		}

		int maxLoop = dellist.size();
		for(int i = 0; i != maxLoop; ++i){
			PubSubId* pubsubId = dellist.get(i);
			__disconnect(pubsubId);
		}
	}

	// clear all
	while(!isClearedAll()){
		clearDeleteList();

		Os::usleep(1000 * 10);
	}
}

void WalletConnectionManager::disconnect(const PubSubId *pubsubId) {
	StackUnlocker __unlock(this->mutex, __FILE__, __LINE__);

	__disconnect(pubsubId);
}

void WalletConnectionManager::__disconnect(const PubSubId *pubsubId) {
	ClientNodeHandshake* handshake = this->clientHandshakeHash.get(pubsubId);
	this->clientHandshakeHash.remove(pubsubId);

	this->dellist.addElement(handshake);
}

void WalletConnectionManager::clearDeleteList() {
	StackUnlocker __unlock(this->mutex, __FILE__, __LINE__);

	int pos = 0;
	int maxLoop = this->dellist.size();
	for(int i = 0; i != maxLoop; ++i){
		ClientNodeHandshake* clientHandshake = this->dellist.get(pos);

		if(clientHandshake->isDeletable()){
			this->dellist.remove(pos);

			// get inner handshake
			P2pHandshake* handshake = clientHandshake->getHandshake();

			// delete wrapper client handshake
			clientHandshake->dispose(true);
			delete clientHandshake;

			// delete inner
			assert(handshake->is2Delete());
			handshake->dispose();
			delete handshake;
		}
		else{
			++pos;
		}
	}

}

int WalletConnectionManager::getNumConnection() const noexcept {
	StackUnlocker __unlock(this->mutex, __FILE__, __LINE__);

	int size = this->clientHandshakeHash.keySet()->size();
	return size;
}

bool WalletConnectionManager::isClearedAll() const noexcept {
	StackUnlocker __unlock(this->mutex, __FILE__, __LINE__);

	return this->dellist.isEmpty();
}

void WalletConnectionManager::addClientHandshake(ClientNodeHandshake* handshake) {
	StackUnlocker __unlock(this->mutex, __FILE__, __LINE__);

	const PubSubId* pubsubId = handshake->getPubsubId();
	this->clientHandshakeHash.put(pubsubId, handshake);
}

bool WalletConnectionManager::hasNodeId(const NodeIdentifier *nodeId) const noexcept {
	bool ret = false;

	StackUnlocker __unlock(this->mutex, __FILE__, __LINE__);

	Iterator<PubSubId>* it = this->clientHandshakeHash.keySet()->iterator(); __STP(it);

	while(it->hasNext()){
		const PubSubId* key = it->next();
		ClientNodeHandshake* clientHandshake = this->clientHandshakeHash.get(key);

		const NodeIdentifier* id = clientHandshake->getNodeId();
		if(nodeId->equals(id)){
			ret = true;
			break;
		}
	}

	return ret;
}

bool WalletConnectionManager::connect(int protocol, const UnicodeString *host, uint32_t port, const NodeIdentifier* nodeId, uint16_t zone, ISystemLogger* logger, const ArrayList<BloomFilter1024>* filters) {
	int ret = false;
	try{
		PubSubId* psId = PubSubId::createNewId(); __STP(psId);
		P2pHandshake *handshake = new P2pHandshake(psId, logger); __STP(handshake);

		handshake->connect(protocol, host, port, this->clientCommandProcessor, true);

		// login
		{
			LoginClientCommand cmd(this->defaultZone);
			// filter
			{
				int maxLoop = filters->size();
				for(int i = 0; i != maxLoop; ++i){
					BloomFilter1024* f = filters->get(i);
					cmd.addBloomFilter(f);
				}
			}
			cmd.sign(this->source);


			AbstractCommandResponse* response = handshake->publishCommand(&cmd); __STP(response);
			OkPubsubResponse* okResponce = dynamic_cast<OkPubsubResponse*>(response);

			ExceptionThrower<P2pClientConnectionException>::throwExceptionIfCondition(okResponce == nullptr, L"Failed in login.", __FILE__, __LINE__);
		}

		ClientNodeHandshake* clientHandshake = new ClientNodeHandshake(__STP_MV(handshake), zone, nodeId); __STP(clientHandshake);
		addClientHandshake(__STP_MV(clientHandshake));

		ret = true;
	}
	catch(Exception* e){
		__STP(e);
		logger->logException(e);
	}

	return ret;
}

ArrayList<NodeIdentifier>* WalletConnectionManager::getNodeIdList() const noexcept {
	StackUnlocker __unlock(this->mutex, __FILE__, __LINE__);

	ArrayList<NodeIdentifier>* list = new ArrayList<NodeIdentifier>();

	Iterator<PubSubId>* it = this->clientHandshakeHash.keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const PubSubId* pubsubId = it->next();
		ClientNodeHandshake* handshake = this->clientHandshakeHash.get(pubsubId);

		const NodeIdentifier* nodeId = handshake->getNodeId();
		list->addElement(dynamic_cast<NodeIdentifier*>(nodeId->copyData()));
	}

	return list;
}

ClientNodeHandshake* WalletConnectionManager::getClientHandshakeByNodeId(const NodeIdentifier *nodeId) const noexcept {
	StackUnlocker __unlock(this->mutex, __FILE__, __LINE__);

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

} /* namespace codablecash */
