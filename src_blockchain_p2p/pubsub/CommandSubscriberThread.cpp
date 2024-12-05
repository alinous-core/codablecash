/*
 * CommandSubscriberThread.cpp
 *
 *  Created on: 2023/01/07
 *      Author: iizuka
 */

#include "pubsub/CommandSubscriberThread.h"

#include "pubsub/CommandSubscriber.h"
#include "pubsub/PubsubCommandException.h"
#include "pubsub/PubsubUtils.h"
#include "pubsub/IPubsubCommandListner.h"

#include "pubsub_cmd/AbstractPubSubCommand.h"
#include "pubsub_cmd/AbstractCommandResponse.h"

#include "ipconnect/IClientSocket.h"

#include "base/Exception.h"
#include "base/StackRelease.h"

#include "base_io/ByteBuffer.h"

#include "bc/ISystemLogger.h"

#include "osenv/funcs.h"

namespace codablecash {

CommandSubscriberThread::CommandSubscriberThread(CommandSubscriber* subscriber, const UnicodeString* name)
		: AbstractThreadRunner(name) {
	assert(subscriber != nullptr);
	this->subscriber = subscriber;

#ifdef __DEBUG__
	this->thread_ends = false;
#endif // __DEBUG__
}

CommandSubscriberThread::~CommandSubscriberThread() {
#ifdef __DEBUG__
	assert(this->subscriber != nullptr);
	assert(this->thread_ends == true);
#endif // __DEBUG__
	this->subscriber = nullptr;
}

void CommandSubscriberThread::process() noexcept {
	ISystemLogger* logger = this->subscriber->getLogger();
	IClientSocket* socket = this->subscriber->getSocket();

	int sec = CommandSubscriberThread::__READ_POLL_MILLS / 1000;
	int usec = (CommandSubscriberThread::__READ_POLL_MILLS % 1000) * 1000;

	while(this->subscriber->isRunning()){
		// poling
		int ret = socket->readPool(sec, usec);
		if(ret > 0){
			try{
				checkSocketConnected(socket);
				handleCommand(socket);
			}
			catch(Exception* e){
				logger->logException(e);

				IPubsubCommandListner* listner = this->subscriber->getListner();
				listner->fireOnExeptionOnSubscriber(e);

				delete e;

				this->subscriber->closeSocket();
				break;
			}
		}
	}

	IPubsubCommandListner* listner = this->subscriber->getListner();
	if(listner != nullptr){
		listner->fireOnSubscribeEnds();
	}

	this->subscriber->setRunning(false);
	this->thread_ends = true;
}

void CommandSubscriberThread::checkSocketConnected(IClientSocket *socket) {
	if(!socket->isConnected()){
		throw new PubsubCommandException(L"Connection is closed by client.", __FILE__, __LINE__);
	}
}

void CommandSubscriberThread::handleCommand(IClientSocket *socket) {
	// check by command size
	char tmp[4]{};
	int n = socket->read(tmp, sizeof(tmp));

	if(n == 0){
		Os::usleep(100 * 1000);
		return; // ignore 0 byte
	}
	else if(n < 4){
		throw new PubsubCommandException(L"Wrong command binary.", __FILE__, __LINE__);
	}

	ByteBuffer* b = ByteBuffer::wrapWithEndian((const uint8_t*)tmp, 4, true); __STP(b);
	int estimatedSize = b->getInt();
	if(estimatedSize > CommandSubscriberThread::MAX_COMMAND_SIZE || estimatedSize < 0){
		throw new PubsubCommandException(L"Too big command binary.", __FILE__, __LINE__);
	}

	ByteBuffer* buff = PubsubUtils::readAll(socket, estimatedSize); __STP(buff);
	buff->position(0);

	AbstractPubSubCommand* cmd = AbstractPubSubCommand::createFromBinary(buff); __STP(cmd);
	IPubsubCommandListner* listner = this->subscriber->getListner();

	AbstractCommandResponse* response = cmd->execute(this->subscriber->getPubsubId() ,listner); __STP(response);
	response->sendCommand(socket);
}

} /* namespace codablecash */
