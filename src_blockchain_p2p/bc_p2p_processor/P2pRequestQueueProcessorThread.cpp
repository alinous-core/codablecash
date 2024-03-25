/*
 * P2pRequestQueueProcessorThread.cpp
 *
 *  Created on: 2023/09/25
 *      Author: iizuka
 */

#include "bc_p2p_processor/P2pRequestQueueProcessorThread.h"
#include "bc_p2p_processor/P2pRequestQueueProcessor.h"

#include "bc_p2p_cmd_node/AbstractNodeCommand.h"

#include "bc/ISystemLogger.h"

#include "base_thread/SynchronizedLock.h"
#include "base_thread/StackUnlocker.h"

#include "base/StackRelease.h"

#include "command_queue/CommandQueueData.h"

#include "command_queue_cmd/AbstructNodeQueueCommand.h"

#include "bc_p2p/BlochchainP2pManager.h"

#include "bc_p2p/BlockchainNodeHandshake.h"

#include "pubsub/P2pHandshake.h"

#include "base/Exception.h"

#include "command_queue/CommandQueue.h"

namespace codablecash {

P2pRequestQueueProcessorThread::P2pRequestQueueProcessorThread(BlochchainP2pManager* p2pManager, P2pRequestQueueProcessor* queue, bool suspend, ISystemLogger* logger, const UnicodeString* name)
		: AbstractThreadRunner(name) {
	this->p2pManager = p2pManager;
	this->logger = logger;
	this->queue = queue;
	this->running = false;
	this->suspend = suspend;
	this->status = STATUS_STOPPED;

	this->lock = new SynchronizedLock();
}

P2pRequestQueueProcessorThread::~P2pRequestQueueProcessorThread() {
	delete this->lock;
}

void P2pRequestQueueProcessorThread::process() noexcept {
	while(isRunning() || !this->queue->isEmpty()){
		CommandQueueData* cmd = nullptr;

		{
			StackUnlocker unlocker(lock);

			this->status = STATUS_RUNNING;

			if(!this->suspend && !this->queue->isEmpty()){
				cmd = this->queue->fetchFirst();
			}
			else{ // empty
				this->status = STATUS_WAITING;
				lock->wait();
				this->status = STATUS_RUNNING;
			}

			lock->notifyAll();
		}

		// process
		if(cmd != nullptr){
			__STP(cmd);
			processCommand(cmd);
		}
	}
}

void P2pRequestQueueProcessorThread::processCommand(CommandQueueData *cmd) {
	AbstructNodeQueueCommand* command = cmd->getCommand();
	const PubSubId* pubsubId = cmd->getPubSubId();

	try{
		command->execute(pubsubId, this->queue);
	}
	catch(Exception* e){
		this->logger->logException(e);
		delete e;
	}
}

void P2pRequestQueueProcessorThread::setRunning(bool bl) noexcept {
	StackUnlocker unlocker(this->lock);

	this->running = bl;
	this->lock->notifyAll();
}

bool P2pRequestQueueProcessorThread::isRunning() const noexcept {
	StackUnlocker unlocker(this->lock);

	return this->running;
}
/*
void P2pRequestQueueProcessorThread::setSuspend(bool suspend) {
	StackUnlocker unlocker(this->lock);

	__setSuspend(suspend);
}
*/

void P2pRequestQueueProcessorThread::__setSuspend(bool suspend) {
	this->suspend = suspend;
	this->lock->notifyAll();
}

int P2pRequestQueueProcessorThread::getStatus() const noexcept {
	StackUnlocker unlocker(this->lock);

	return this->status;
}

/*
void P2pRequestQueueProcessorThread::notifyAll() {
	StackUnlocker unlocker(this->lock);
	this->lock->notifyAll();
}*/

} /* namespace codablecash */
