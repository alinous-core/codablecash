/*
 * CommandProcessUnit.cpp
 *
 *  Created on: 2023/08/13
 *      Author: iizuka
 */

#include "procesor_multi/CommandProcessUnit.h"
#include "procesor_multi/MultipleCommandProcessor.h"

#include "base_thread/SynchronizedLock.h"
#include "base_thread/StackUnlocker.h"

#include "procesor/ICommandMessage.h"

#include "base/StackRelease.h"
#include "base/Exception.h"

#include "bc/ISystemLogger.h"


namespace codablecash {

CommandProcessUnit::CommandProcessUnit(MultipleCommandProcessor* processor, ISystemLogger* logger, const UnicodeString* name)
		: AbstractThreadRunner(name) {
	this->processor = processor;
	this->logger = logger;
	this->running = false;
	this->status = STATUS_STOPPED;
	this->lock = new SynchronizedLock();
}

CommandProcessUnit::~CommandProcessUnit() {
	this->processor = nullptr;
	this->logger = nullptr;
	delete this->lock;
}

void CommandProcessUnit::process() noexcept {
	while(isRunning() || !checkListIsEmpty()){
		ICommandMessage* cmd = nullptr;

		{
			StackUnlocker unlocker(lock, __FILE__, __LINE__);

			this->status = STATUS_RUNNING;

			if(!this->list.isEmpty()){
				cmd = this->list.remove(0);
			}
			else{ // empty
				this->status = STATUS_WAITING;
				if(this->running){
					lock->wait();
				}
				this->status = STATUS_RUNNING;
			}

			lock->notifyAll();
		}

		// process
		if(cmd != nullptr){
			processCommand(cmd);
		}
	}
	this->status = STATUS_STOPPED;
}

void CommandProcessUnit::processCommand(ICommandMessage *cmd) {
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

void CommandProcessUnit::setRunning(bool bl) noexcept {
	StackUnlocker unlocker(this->lock, __FILE__, __LINE__);
	this->running = bl;
	this->lock->notifyAll();
}

bool CommandProcessUnit::isRunning() const noexcept {
	StackUnlocker unlocker(this->lock, __FILE__, __LINE__);

	return this->running;
}

int CommandProcessUnit::getStatus() const noexcept {
	StackUnlocker unlocker(lock, __FILE__, __LINE__);

	return this->status;
}

bool CommandProcessUnit::checkListIsEmpty() {
	StackUnlocker unlocker(this->lock, __FILE__, __LINE__);

	return this->list.isEmpty();
}

void CommandProcessUnit::addCommandMessage(ICommandMessage *cmd) noexcept {
	StackUnlocker unlocker(this->lock, __FILE__, __LINE__);
	this->list.addElement(cmd);
	this->lock->notifyAll();
}

void CommandProcessUnit::insertCommandMessage(ICommandMessage *cmd, int pos) noexcept {
	StackUnlocker unlocker(this->lock, __FILE__, __LINE__);

	this->list.addElement(cmd, pos);
	this->lock->notifyAll();
}

} /* namespace codablecash */
