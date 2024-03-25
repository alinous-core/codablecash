/*
 * CommandSubscriber.h
 *
 *  Created on: 2023/01/07
 *      Author: iizuka
 */

#ifndef PUBSUB_COMMANDSUBSCRIBER_H_
#define PUBSUB_COMMANDSUBSCRIBER_H_

namespace alinous {
class SysMutex;
}
using namespace alinous;

namespace codablecash {

class IClientSocket;
class CommandSubscriberThread;
class PubSubId;
class ISystemLogger;
class IPubsubCommandListner;

class CommandSubscriber {
public:
	CommandSubscriber(IClientSocket* socket, const PubSubId* pubsubId, ISystemLogger* logger);
	virtual ~CommandSubscriber();

	void closeSocket() noexcept;

	void startSubscribe();
	void stopSubscribe() noexcept;

	IClientSocket* getSocket() const noexcept {
		return socket;
	}
	IClientSocket* moveSocket() noexcept {
		IClientSocket* s = this->socket;
		this->socket = nullptr;
		return s;
	}

	bool isRunning() const noexcept;
	void setRunning(bool bl) noexcept;

	const PubSubId* getPubsubId() const noexcept {
		return pubsubId;
	}

	ISystemLogger* getLogger() const noexcept {
		return logger;
	}

	void setListner(IPubsubCommandListner *listner) {
		this->listner = listner;
	}
	IPubsubCommandListner* getListner() const noexcept {
		return listner;
	}

private:
	PubSubId* pubsubId;
	IClientSocket* socket;
	CommandSubscriberThread* thread;

	bool running;
	SysMutex* runningFlagMutex;

	ISystemLogger* logger;

	IPubsubCommandListner* listner;
};

} /* namespace codablecash */

#endif /* PUBSUB_COMMANDSUBSCRIBER_H_ */
