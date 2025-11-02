/*
 * ClientCommandsQueueProcessor.cpp
 *
 *  Created on: Mar 26, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_processor/ClientCommandsQueueProcessor.h"
#include "bc_wallet_net_cmd_queue/ClientCommandsQueue.h"
#include "bc_wallet_net_processor/NetworkClientCommandProcessorThread.h"

#include "base_thread/SysMutex.h"
#include "base_thread/StackUnlocker.h"

#include "base/UnicodeString.h"

#include "osenv/funcs.h"

namespace codablecash {

ClientCommandsQueueProcessor::ClientCommandsQueueProcessor(NetworkWallet* wallet, const File* baseDir, ISystemLogger* logger) {
	this->wallet = wallet;
	this->mutex = new SysMutex();
	this->queue = new ClientCommandsQueue(baseDir);
	this->thread = nullptr;
	this->logger = logger;
}

ClientCommandsQueueProcessor::~ClientCommandsQueueProcessor() {
	close();

	delete this->mutex;
	this->logger = nullptr;
}

void ClientCommandsQueueProcessor::createBlankDatabase() {
	this->queue->createBlankDatabase();
}

void ClientCommandsQueueProcessor::open(bool suspend) {
	this->queue->open();

	UnicodeString name(L"");
	name.append(THREAD_NAME);

	this->thread = new NetworkClientCommandProcessorThread(this->wallet, this, suspend, this->logger, &name);
	this->thread->setRunning(true);
	this->thread->start();

	while(this->thread->getStatus() != NetworkClientCommandProcessorThread::STATUS_WAITING) Os::usleep(1000);
}

void ClientCommandsQueueProcessor::close() {
	if(this->thread != nullptr){
		this->thread->setRunning(false);
		this->thread->join();

		delete this->thread;
		this->thread = nullptr;
	}

	if(this->queue != nullptr){
		StackUnlocker __lock(this->mutex, __FILE__, __LINE__);

		this->queue->close();
		delete this->queue;
		this->queue = nullptr;
	}
}

bool ClientCommandsQueueProcessor::isEmpty() const {
	StackUnlocker __lock(this->mutex, __FILE__, __LINE__);

	return this->queue->isEmpty();
}

AbstractClientQueueCommand* ClientCommandsQueueProcessor::fetchFirst() const {
	StackUnlocker __lock(this->mutex, __FILE__, __LINE__);

	return this->queue->fetchFirst();
}

bool ClientCommandsQueueProcessor::__isSuspended() const {
	return this->thread->__isSuspended();
}

SynchronizedLock* ClientCommandsQueueProcessor::getSynchrinizedLock() const noexcept {
	return this->thread->getSynchronizedLock();
}

void ClientCommandsQueueProcessor::__setSuspend(bool suspend) {
	this->thread->__setSuspend(suspend);
}

void ClientCommandsQueueProcessor::addCommand(const AbstractClientQueueCommand *cmd) {
	StackUnlocker __lock(this->mutex, __FILE__, __LINE__);

	if(this->queue != nullptr){
		 this->queue->addCommand(cmd);
	}
}

} /* namespace codablecash */
