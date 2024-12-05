/*
 * CommandPublisher.cpp
 *
 *  Created on: 2023/01/07
 *      Author: iizuka
 */

#include "pubsub/CommandPublisher.h"
#include "pubsub/PubSubId.h"
#include "pubsub/PubsubCommandException.h"
#include "pubsub/PubsubUtils.h"
#include "pubsub/CommandPublisherSetting.h"
#include "pubsub/PubsubNetworkException.h"

#include "pubsub_cmd/AbstractPubSubCommand.h"
#include "pubsub_cmd/AbstractCommandResponse.h"
#include "pubsub_cmd/EndHandshakeCommand.h"

#include "ipconnect/IClientSocket.h"

#include "osenv/funcs.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"

#include "base_thread/SysMutex.h"
#include "base_thread/StackUnlocker.h"

#include "bc/ISystemLogger.h"
#include "bc/ExceptionThrower.h"


namespace codablecash {

CommandPublisher::CommandPublisher(IClientSocket* socket, const PubSubId* pubsubId) {
	this->socket = socket;
	this->pubsubId = new PubSubId(*pubsubId);
	this->publishLock = new SysMutex();

	this->timestamp = Os::getTimestampLong();

	// setting
	CommandPublisherSetting* setting = CommandPublisherSetting::getInstance();

	this->poolRetry = setting->getPoolRetry();
	this->waitSec = setting->getWaitSec();
}

CommandPublisher::~CommandPublisher() {
	close();

	delete this->pubsubId;
	delete this->publishLock;
}

void CommandPublisher::sendEndConnectionCommand(ISystemLogger* logger) noexcept {
	if(this->socket != nullptr){
		try{
			EndHandshakeCommand cmd;
			AbstractCommandResponse* res = publishCommand(&cmd); __STP(res);

			UnicodeString* message = res->toString(); __STP(message);
			logger->debugLog(ISystemLogger::DEBUG_NODE_TRANSFER_RESPONSE, message, __FILE__, __LINE__);
		}
		catch(Exception* e){
			logger->logException(e);
			delete e;
		}
	}
}

AbstractCommandResponse* CommandPublisher::publishCommand(const AbstractPubSubCommand *cmd) {
	StackUnlocker __lock(this->publishLock, __FILE__, __LINE__);

	int size = cmd->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	cmd->toBinary(buff);

	doPublishCommand(buff);

	// pool while
	int numFd = 0;
	int retry = 0;
	while(numFd < 1){
		// publiser side error
		ExceptionThrower<PubsubCommandException>::throwExceptionIfCondition(retry >= this->poolRetry, L"Server does not respond.", __FILE__, __LINE__);
		numFd = this->socket->readPool(this->waitSec, 0);

		retry++;
	}

	ByteBuffer* data = reveiveResponseData(); __STP(data);
	AbstractCommandResponse* response = AbstractCommandResponse::createFromBinary(data);

	return response;
}

ByteBuffer* CommandPublisher::reveiveResponseData() {
	// check by command response size
	char tmp[4]{};
	int n = this->socket->read(tmp, sizeof(tmp));
	if(n < 4){
		// end connection
		throw new PubsubNetworkException(L"Wrong command response binary.", __FILE__, __LINE__);
	}

	ByteBuffer* b = ByteBuffer::wrapWithEndian((const uint8_t*)tmp, 4, true); __STP(b);
	int estimatedSize = b->getInt();
	if(estimatedSize > CommandPublisher::MAX_COMMAND_RESPONCE_SIZE || estimatedSize < 0){
		throw new PubsubCommandException(L"Too big command binary.", __FILE__, __LINE__);
	}

	ByteBuffer* buff = PubsubUtils::readAll(this->socket, estimatedSize);
	buff->position(0);

	return buff;
}


void CommandPublisher::doPublishCommand(ByteBuffer *buff) {
	buff->position(0);
	int size = buff->limit() + sizeof(uint32_t);

	ByteBuffer* newbuff = ByteBuffer::allocateWithEndian(size, true); __STP(newbuff);
	newbuff->putInt(buff->limit());
	newbuff->put(buff);

	publishCommand(newbuff->array(), size);
}

void CommandPublisher::publishCommand(const uint8_t *buff, int length) {
	// check socket conditions
	ExceptionThrower<PubsubCommandException>::throwExceptionIfCondition(!this->socket->isConnected(), L"", __FILE__, __LINE__);

	int n = this->socket->write((const char*)buff, length);

	ExceptionThrower<PubsubCommandException>::throwExceptionIfCondition(n != length, L"Failed in writing socket data.", __FILE__, __LINE__);
}

void CommandPublisher::close() noexcept {
	if(this->socket != nullptr){
		this->socket->close();
		delete this->socket;
		this->socket = nullptr;
	}
}

} /* namespace codablecash */
