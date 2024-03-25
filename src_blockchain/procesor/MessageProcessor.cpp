/*
 * MessageProcessor.cpp
 *
 *  Created on: 2022/03/07
 *      Author: iizuka
 */

#include "procesor/MessageProcessor.h"
#include "procesor/MessageProcessorThread.h"

#include "base_thread/SynchronizedLock.h"
#include "base_thread/StackUnlocker.h"

#include "osenv/funcs.h"

#include "base/StackRelease.h"

namespace codablecash {

MessageProcessor::MessageProcessor(ISystemLogger* logger) {
	this->lock = new SynchronizedLock();
	this->thread = new MessageProcessorThread(this, logger);
	this->param = nullptr;
	this->acceptCommands = true;
}

MessageProcessor::MessageProcessor(const UnicodeString *name, ISystemLogger* logger) {
	this->lock = new SynchronizedLock();
	this->thread = new MessageProcessorThread(this, name, logger);
	this->param = nullptr;
	this->acceptCommands = true;
}

MessageProcessor::~MessageProcessor() {
	shutdown();

	delete this->lock;
	delete this->thread;

	this->list.deleteElements();
	this->param = nullptr;
}

void MessageProcessor::start() {
	this->thread->setRunning(true);
	this->thread->start();


	while(this->thread->getStatus() == MessageProcessorThread::STATUS_STOPPED) Os::usleep(1000);
}

void MessageProcessor::shutdown() noexcept {
	this->acceptCommands = false;

	if(this->thread != nullptr){
		this->thread->setRunning(false);

		this->thread->join();
		delete this->thread, this->thread = nullptr;
	}
}

void MessageProcessor::addCommandMessage(ICommandMessage* cmd) noexcept {
	__STP(cmd);

	if(this->acceptCommands){
		StackUnlocker unlocker(this->lock);

		this->list.addElement(__STP_MV(cmd));
		this->lock->notifyAll();
	}
}

bool MessageProcessor::__isEmpty() const noexcept {
	return this->list.isEmpty();
}

ICommandMessage* MessageProcessor::__fetch() noexcept {
	ICommandMessage* ret = nullptr;
	if(!this->list.isEmpty()){
		ret = this->list.remove(0);
	}

	return ret;
}

void MessageProcessor::setParam(ICommandParameter *param) noexcept {
	this->param = param;
}

} /* namespace codablecash */
