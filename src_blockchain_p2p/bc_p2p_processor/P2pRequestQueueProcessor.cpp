/*
 * P2pRequestQueueProcessor.cpp
 *
 *  Created on: 2023/09/05
 *      Author: iizuka
 */

#include "bc_p2p_processor/P2pRequestQueueProcessor.h"
#include "bc_p2p_processor/NetworkTransferProcessor.h"
#include "bc_p2p_processor/P2pRequestQueueProcessorThread.h"

#include "command_queue/CommandQueue.h"

#include "base_thread/SysMutex.h"
#include "base_thread/StackUnlocker.h"

#include "bc_p2p/BlochchainP2pManager.h"

#include "osenv/funcs.h"

#include "base/UnicodeString.h"

#include "command_queue/CommandQueueData.h"

#include "bc/CodablecashNodeInstance.h"

#include "base_thread/SynchronizedLock.h"

namespace codablecash {

P2pRequestQueueProcessor::P2pRequestQueueProcessor(const File* baseDir, BlochchainP2pManager* p2pManager, NetworkTransferProcessor* processor, CodablecashNodeInstance *inst, ISystemLogger* logger) {
	this->mutex = new SysMutex();
	this->logger = logger;
	this->inst = inst;
	this->p2pManager = p2pManager;
	this->queue = new CommandQueue(baseDir);
	this->processor = processor;
	this->thread = nullptr;
}

P2pRequestQueueProcessor::~P2pRequestQueueProcessor() {
	close();

	delete this->mutex;
	this->processor = nullptr;
}

void P2pRequestQueueProcessor::createBlankDatabase() {
	this->queue->createBlankDatabase();
}

void P2pRequestQueueProcessor::open(bool suspend) {
	this->queue->open();

	UnicodeString name(L"");

	const UnicodeString* nodeName = this->inst->getNodeName();
	if(nodeName != nullptr){
		name.append(nodeName);
		name.append(L"_");
	}
	name.append(THREAD_NAME);

	this->thread = new P2pRequestQueueProcessorThread(this->p2pManager, this, suspend, this->logger, &name);
	this->thread->setRunning(true);
	this->thread->start();

	while(this->thread->getStatus() != P2pRequestQueueProcessorThread::STATUS_WAITING) Os::usleep(1000);
}

void P2pRequestQueueProcessor::close() {
	if(this->thread != nullptr){
		this->thread->setRunning(false);
		this->thread->join();

		delete this->thread;
		this->thread = nullptr;
	}

	if(this->queue != nullptr){
		this->queue->close();
		delete this->queue;
		this->queue = nullptr;
	}
}

bool P2pRequestQueueProcessor::isEmpty() const {
	StackUnlocker __lock(this->mutex, __FILE__, __LINE__);

	return this->queue->isEmpty();
}

CommandQueueData* P2pRequestQueueProcessor::fetchFirst() const {
	StackUnlocker __lock(this->mutex, __FILE__, __LINE__);

	return this->queue->fetchFirst();
}

void P2pRequestQueueProcessor::put(const CommandQueueData *data) {
	SynchronizedLock* threadLock = this->thread->getSynchronizedLock();

	StackUnlocker unlocker(threadLock, __FILE__, __LINE__);

	{
		StackUnlocker __lock(this->mutex, __FILE__, __LINE__);

		this->queue->addCommand(data);
	}

	threadLock->notifyAll();
}

IPubsubCommandExecutor* P2pRequestQueueProcessor::getExecutor() const noexcept {
	return this->inst;
}

/*
void P2pRequestQueueProcessor::setSuspend(bool suspend) {
	this->thread->setSuspend(suspend);
}
*/
void P2pRequestQueueProcessor::__setSuspend(bool suspend) {
	this->thread->__setSuspend(suspend);
}

bool P2pRequestQueueProcessor::__isSuspended() const noexcept {
	return this->thread->__isSuspended();
}

SynchronizedLock* P2pRequestQueueProcessor::getSynchronizedLock() const noexcept {
	return this->thread->getSynchronizedLock();
}

} /* namespace codablecash */
