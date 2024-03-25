/*
 * MessageProcessorThread.cpp
 *
 *  Created on: 2022/03/07
 *      Author: iizuka
 */

#include "procesor/MessageProcessorThread.h"
#include "procesor/MessageProcessor.h"

#include "base_thread/SynchronizedLock.h"
#include "base_thread/StackUnlocker.h"

#include "osenv/funcs.h"

#include "base/StackRelease.h"
#include "base/Exception.h"

#include "bc/ISystemLogger.h"

using namespace alinous;

namespace codablecash {

MessageProcessorThread::MessageProcessorThread(MessageProcessor* processor, ISystemLogger* logger) {
	this->processor = processor;
	this->running = false;
	this->status = STATUS_STOPPED;
	this->logger = logger;
}

MessageProcessorThread::MessageProcessorThread(MessageProcessor *processor,	const UnicodeString *name, ISystemLogger* logger)
	: AbstractThreadRunner(name) {
	this->processor = processor;
	this->running = false;
	this->status = STATUS_STOPPED;
	this->logger = logger;
}

MessageProcessorThread::~MessageProcessorThread() {
	this->processor = nullptr;
}

void MessageProcessorThread::process() noexcept {
	processLoop();
}

void MessageProcessorThread::processLoop() noexcept {
	SynchronizedLock* lock = this->processor->getLock();

	while(isRunning() || !checkListIsEmpty()){
		ICommandMessage* cmd = nullptr;

		{
			StackUnlocker unlocker(lock);

			this->status = STATUS_RUNNING;

			if(!this->processor->__isEmpty()){
				cmd = this->processor->__fetch();
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
			processCommand(cmd);
		}
	}
}

void MessageProcessorThread::setRunning(bool bl) noexcept {
	SynchronizedLock* lock = this->processor->getLock();
	StackUnlocker unlocker(lock);

	this->running = bl;
	lock->notifyAll();
}

bool MessageProcessorThread::isRunning() const noexcept {
	SynchronizedLock* lock = this->processor->getLock();
	StackUnlocker unlocker(lock);

	return this->running;
}

int MessageProcessorThread::getStatus() const noexcept {
	SynchronizedLock* lock = this->processor->getLock();
	StackUnlocker unlocker(lock);

	return this->status;
}

void MessageProcessorThread::processCommand(ICommandMessage* cmd) {
	__STP(cmd);

	ICommandParameter* param = this->processor->getParam();

	try{
		cmd->execute(param);
	}
	catch(Exception* e){
		this->logger->logException(e);
		delete e;
	}

}

bool MessageProcessorThread::checkListIsEmpty() {
	SynchronizedLock* lock = this->processor->getLock();
	StackUnlocker unlocker(lock);

	return this->processor->__isEmpty();
}

} /* namespace codablecash */
