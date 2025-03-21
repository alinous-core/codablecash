/*
 * WalletConnectionManager.cpp
 *
 *  Created on: Feb 21, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net/WalletConnectionManager.h"

#include "bc_network/NodeIdentifierSource.h"

#include "pubsub/PubSubId.h"
#include "pubsub/P2pHandshake.h"

#include "bc_p2p/ClientNodeHandshake.h"

#include "base_thread/SysMutex.h"
#include "base_thread/StackUnlocker.h"

#include "base/StackRelease.h"
#include "base/Exception.h"

#include "osenv/funcs.h"

#include "bc/ISystemLogger.h"

#include "bc_p2p_cmd/LoginClientCommand.h"

#include "bc_wallet_net/NetworkClientCommandProcessor.h"
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

	this->list.addElement(handshake);
}

void WalletConnectionManager::clearDeleteList() {
	StackUnlocker __unlock(this->mutex, __FILE__, __LINE__);

	int pos = 0;
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		ClientNodeHandshake* handshake = this->list.get(pos);

		if(handshake->isDeletable()){
			this->list.remove(pos);
			handshake->dispose(true);
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

	return this->list.isEmpty();
}

bool WalletConnectionManager::connect(int protocol, const UnicodeString *host, uint32_t port, ISystemLogger* logger) {
	int ret = false;
	try{
		PubSubId* psId = PubSubId::createNewId(); __STP(psId);
		P2pHandshake *handshake = new P2pHandshake(psId, logger); __STP(handshake);

		handshake->connect(protocol, host, port, this->clientCommandProcessor, true);

		// login
		{
			LoginClientCommand cmd(this->defaultZone);
			cmd.sign(this->source);
			AbstractCommandResponse* response = handshake->publishCommand(&cmd); __STP(response);


		}
		// FIXME connect


		ret = true;
	}
	catch(Exception* e){
		__STP(e);
		logger->logException(e);
	}

	return ret;
}

} /* namespace codablecash */
