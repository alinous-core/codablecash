/*
 * CommandSubscriber.cpp
 *
 *  Created on: 2023/01/07
 *      Author: iizuka
 */

#include "pubsub/CommandSubscriber.h"

#include "pubsub/CommandSubscriberThread.h"
#include "pubsub/PubSubId.h"

#include "ipconnect/IClientSocket.h"

#include "base_thread/SysMutex.h"
#include "base_thread/StackUnlocker.h"


namespace codablecash {

CommandSubscriber::CommandSubscriber(IClientSocket* socket, const PubSubId* pubsubId, ISystemLogger* logger) {
	this->socket = socket;
	this->thread = nullptr;
	this->running = false;
	this->runningFlagMutex = new SysMutex();

	this->pubsubId = new PubSubId(*pubsubId);

	this->logger = logger;
	this->listner = nullptr;
}

CommandSubscriber::~CommandSubscriber() {
	stopSubscribe();

	closeSocket();

	delete this->runningFlagMutex;

	delete this->pubsubId;

	this->logger = nullptr;
	this->listner = nullptr;
}

void CommandSubscriber::closeSocket() noexcept {
	if(this->socket != nullptr){
		this->socket->close();
		delete this->socket;
		this->socket = nullptr;
	}
}

void CommandSubscriber::startSubscribe() {
	if(!isRunning()){
		setRunning(true);

		this->thread = new CommandSubscriberThread(this);
		this->thread->start();
	}
}

void CommandSubscriber::stopSubscribe() noexcept {
	if(this->thread != nullptr){
		setRunning(false);
		this->thread->join();

		delete this->thread;
		this->thread = nullptr;
	}
}

bool CommandSubscriber::isRunning() const noexcept {
	StackUnlocker __lock(this->runningFlagMutex);
	bool bl = this->running;

	return bl;
}

void CommandSubscriber::setRunning(bool bl) noexcept {
	StackUnlocker __lock(this->runningFlagMutex);

	this->running = bl;
}

} /* namespace codablecash */
