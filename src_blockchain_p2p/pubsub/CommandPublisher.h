/*
 * CommandPublisher.h
 *
 *  Created on: 2023/01/07
 *      Author: iizuka
 */

#ifndef PUBSUB_COMMANDPUBLISHER_H_
#define PUBSUB_COMMANDPUBLISHER_H_
#include <cstdint>

namespace alinous {
class ByteBuffer;
class SysMutex;
}
using namespace alinous;

namespace codablecash {

class AbstractPubSubCommand;
class AbstractCommandResponse;
class ISystemLogger;

class IClientSocket;
class PubSubId;

class CommandPublisher {
public:
	static const constexpr int MAX_COMMAND_RESPONCE_SIZE{20 * 1000 * 1000}; // 20MB

	CommandPublisher(IClientSocket* socket, const PubSubId* pubsubId);
	virtual ~CommandPublisher();

	void sendEndConnectionCommand(ISystemLogger* logger) noexcept;

	AbstractCommandResponse* publishCommand(const AbstractPubSubCommand* cmd);
	void close() noexcept;

	const PubSubId* getPubsubId() const noexcept {
		return pubsubId;
	}

	uint64_t getTimestamp() const noexcept {
		return timestamp;
	}

	IClientSocket* moveSocket() noexcept {
		IClientSocket* s = this->socket;
		this->socket = nullptr;
		return s;
	}

private:
	void doPublishCommand(ByteBuffer* buff);
	void publishCommand(const uint8_t* buff, int length);

	ByteBuffer* reveiveResponseData();

private:
	PubSubId* pubsubId;
	IClientSocket* socket;
	SysMutex* publishLock;

	uint64_t timestamp;
};

} /* namespace codablecash */

#endif /* PUBSUB_COMMANDPUBLISHER_H_ */
