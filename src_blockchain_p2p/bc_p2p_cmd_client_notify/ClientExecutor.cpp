/*
 * ClientExecutor.cpp
 *
 *  Created on: 2023/10/05
 *      Author: iizuka
 */

#include "bc_p2p_cmd_client_notify/ClientExecutor.h"
#include "bc_p2p_cmd_client_notify/IClientNotifyListner.h"

#include "pubsub_cmd/OkPubsubResponse.h"

#include "base_thread/SysMutex.h"
#include "base_thread/StackUnlocker.h"

namespace codablecash {

ClientExecutor::ClientExecutor() {
	this->mutex = new SysMutex();
	this->listners = new ArrayList<IClientNotifyListner>();
}

ClientExecutor::~ClientExecutor() {
	delete this->listners;
	delete this->mutex;
}

void ClientExecutor::addListner(IClientNotifyListner *listner) noexcept {
	StackUnlocker lock(this->mutex, __FILE__, __LINE__);
	this->listners->addElement(listner);
}

IClientNotifyListner* ClientExecutor::removeListner(IClientNotifyListner *listner) noexcept {
	StackUnlocker lock(this->mutex, __FILE__, __LINE__);

	int index = this->listners->indexOfPtr(listner);
	return this->listners->remove(index);
}

void ClientExecutor::clearAllListners() noexcept {
	StackUnlocker lock(this->mutex, __FILE__, __LINE__);

	this->listners->reset();
}

AbstractCommandResponse* ClientExecutor::fireOnNewTransaction(const PubSubId *pubsubId, const TransactionTransferData *data) {
	StackUnlocker lock(this->mutex, __FILE__, __LINE__);
	int maxLoop = this->listners->size();
	for(int i = 0; i != maxLoop; ++i){
		IClientNotifyListner* listner = this->listners->get(i);

		listner->onNewTransaction(pubsubId, data);
	}

	return new OkPubsubResponse();
}

} /* namespace codablecash */
