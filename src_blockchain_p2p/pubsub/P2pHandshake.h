/*
 * P2pHandshake.h
 *
 *  Created on: 2023/01/10
 *      Author: iizuka
 */

#ifndef PUBSUB_P2PHANDSHAKE_H_
#define PUBSUB_P2PHANDSHAKE_H_

#include "pubsub/IPubsubCommandListner.h"

namespace alinous {
class UnicodeString;
class SysMutex;
}
using namespace alinous;

namespace codablecash {

class PubSubId;
class CommandSubscriber;
class CommandPublisher;
class IClientSocket;
class AbstractPubSubCommand;
class AbstractCommandResponse;
class ISystemLogger;
class P2pServerConnectionManager;

class P2pHandshake : public IPubsubCommandListner {
public:
	P2pHandshake(CommandPublisher *publisher, CommandSubscriber *subscriber, ISystemLogger* logger, P2pServerConnectionManager* connectionManager, IPubsubCommandExecutor* executor);
	explicit P2pHandshake(const PubSubId* psId, ISystemLogger* logger);
	virtual ~P2pHandshake();

	void init(P2pServerConnectionManager* connectionManager, IPubsubCommandExecutor* executor);

	void dispose() noexcept;
	void sendEndConnectionCommand();

	/**
	 * client side connection
	 */
	void connect(int protocol, const UnicodeString* host, int port, IPubsubCommandListner *listner, bool startThread);
	void connectIpV6(const UnicodeString* host, int port, IPubsubCommandListner *listner, bool startThread);
	void connectIpV6(const UnicodeString* host, int port);
	void connectIpV4(const UnicodeString* host, int port, IPubsubCommandListner *listner, bool startThread);
	void connectIpV4(const UnicodeString* host, int port);

	AbstractCommandResponse* publishCommand(const AbstractPubSubCommand* cmd);

	void startSubscribe();

	/**
	 * listner command
	 */
	virtual void fireExecuteCommand(const AbstractPubSubCommand* cmd);
	virtual void fireOnExeptionOnSubscriber(const Exception* e);
	virtual void fireOnSubscribeEnds();
	virtual IPubsubCommandExecutor* getExecutor() const noexcept;

	bool is2Delete() const noexcept;
	void addRef() noexcept;
	void decRef() noexcept;
	int getRef() const noexcept;

	void setDisabled(bool bl) noexcept;

	const PubSubId* getPubsubid() const noexcept {
		return pubsubid;
	}

#ifdef __DEBUG__
	CommandPublisher* getPublisher(){
		return this->publisher;
	}
#endif

protected:
	void disposePublisher() noexcept;
	void disposeSubscriber() noexcept;

	void connectSubscriber(IClientSocket* con, IPubsubCommandListner *listner, bool startThread);
	void connectPublisher(IClientSocket* con);

	void checkConnection(IClientSocket* con);

private:
	PubSubId* pubsubid;

	CommandSubscriber* subscriber;
	CommandPublisher* publisher;
	SysMutex* publisherLock;
	SysMutex* subscriberLock;

	ISystemLogger* logger;

	bool disabled;
	int useRef;
	SysMutex* useRefLock;

	P2pServerConnectionManager* connectionManager;
	IPubsubCommandExecutor* executor;
};

} /* namespace codablecash */

#endif /* PUBSUB_P2PHANDSHAKE_H_ */
