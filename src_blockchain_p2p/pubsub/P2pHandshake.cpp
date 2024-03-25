/*
 * P2pHandshake.cpp
 *
 *  Created on: 2023/01/10
 *      Author: iizuka
 */

#include "pubsub/P2pHandshake.h"

#include "pubsub/PubSubId.h"
#include "pubsub/CommandSubscriber.h"
#include "pubsub/CommandPublisher.h"
#include "pubsub/HandshakeConnectionException.h"

#include "pubsub_cmd/AbstractPubSubCommand.h"

#include "p2pserver/ConnectSubscriberCommand.h"
#include "p2pserver/ConnectPublisherCommand.h"
#include "p2pserver/P2pServerConnectionManager.h"

#include "ipconnect/IpV6ClientConnection.h"

#include "base/StackRelease.h"

#include "base_thread/SysMutex.h"
#include "base_thread/StackUnlocker.h"

#include "bc_p2p_cmd/LoginPubSubCommand.h"
#include "bc_p2p_cmd/LoginClientCommand.h"

#include "ipconnect/IpV4ClientConnection.h"
namespace codablecash {

P2pHandshake::P2pHandshake(CommandPublisher *publisher,	CommandSubscriber *subscriber, ISystemLogger* logger
		, P2pServerConnectionManager* connectionManager, IPubsubCommandExecutor* executor) {
	this->subscriber = subscriber;
	this->subscriber->setListner(this);

	this->publisher = publisher;

	const PubSubId* id = publisher->getPubsubId();
	this->pubsubid = new PubSubId(*id);

	this->logger = logger;
	this->publisherLock = new SysMutex();
	this->subscriberLock = new SysMutex();

	this->disabled = false;
	this->useRef = 0;
	this->useRefLock = new SysMutex();

	this->connectionManager = connectionManager;
	this->executor = executor;
}

P2pHandshake::P2pHandshake(const PubSubId* psId, ISystemLogger* logger) {
	this->pubsubid = new PubSubId(*psId);

	this->subscriber = nullptr;
	this->publisher = nullptr;

	this->logger = logger;
	this->publisherLock = new SysMutex();
	this->subscriberLock = new SysMutex();

	this->disabled = false;
	this->useRef = 0;
	this->useRefLock = new SysMutex();
	this->connectionManager = nullptr;
	this->executor = nullptr;
}

P2pHandshake::~P2pHandshake() {
	dispose();

	delete this->pubsubid;
	this->logger = nullptr;
	delete this->publisherLock;
	delete this->subscriberLock;

	delete this->useRefLock;
}

void P2pHandshake::init(P2pServerConnectionManager *connectionManager, IPubsubCommandExecutor *executor) {
	this->connectionManager = connectionManager;
	this->executor = executor;
}

void P2pHandshake::dispose() noexcept {
	disposePublisher();
	disposeSubscriber();

	setDisabled(true);
}

void P2pHandshake::disposeSubscriber() noexcept {
	StackUnlocker __lock(this->subscriberLock);

	CommandSubscriber* sub = nullptr;
	if(this->subscriber != nullptr){
		sub = this->subscriber;
		this->subscriber = nullptr;
	}

	if(sub != nullptr){
		sub->stopSubscribe();
		delete sub;
	}
}

void P2pHandshake::disposePublisher() noexcept {
	StackUnlocker __lock(this->publisherLock);

	CommandPublisher* pub = nullptr;
	if(this->publisher != nullptr){
		pub = this->publisher;
		this->publisher = nullptr;
	}

	if(pub != nullptr){
		this->subscriber->setRunning(false);

		delete pub;
	}
}

void P2pHandshake::sendEndConnectionCommand() {
	if(this->publisher != nullptr){
		this->publisher->sendEndConnectionCommand(this->logger);
	}
}

void P2pHandshake::fireExecuteCommand(const AbstractPubSubCommand *cmd) {
	int type = cmd->getType();

	if(type == AbstractPubSubCommand::TYPE_END_HANDSHAKE){
		// callback on P2P handshake ends
		if(this->connectionManager != nullptr){
			this->connectionManager->fireHandshakeEnds(this);
		}

		disposePublisher();
		this->disabled = true;
	}
	else if(type == AbstractPubSubCommand::TYPE_LOGIN){
		const LoginPubSubCommand* loginCommand = dynamic_cast<const LoginPubSubCommand*>(cmd);
		if(this->connectionManager != nullptr){
			this->connectionManager->fireLoginHandshake(this, loginCommand);
		}
	}
	else if(type == AbstractPubSubCommand::TYPE_CLIENT_LOGIN){
		const LoginClientCommand* loginClientCommand = dynamic_cast<const LoginClientCommand*>(cmd);
		if(this->connectionManager != nullptr){
			this->connectionManager->fireClientLoginHandshake(this, loginClientCommand);
		}
	}
}

void P2pHandshake::fireOnSubscribeEnds() {
	if(this->connectionManager != nullptr){
		this->connectionManager->fireHandshakeEnds(this);
	}

	disposePublisher();
	this->disabled = true;
}

void P2pHandshake::fireOnExeptionOnSubscriber(const Exception *e) {
	// callback on P2P handshake ends
	if(this->connectionManager != nullptr){
		this->connectionManager->fireHandshakeEndsWithExeption(this, e);
	}

	disposePublisher();
	this->disabled = true;
}


void P2pHandshake::connectIpV6(const UnicodeString *host, int port) {
	connectIpV6(host, port, this, true);
}

void P2pHandshake::connectIpV6(const UnicodeString *host, int port, IPubsubCommandListner *listner, bool startThread) {
	// subscriber
	{
		IpV6ClientConnection* con = new IpV6ClientConnection(); __STP(con);
		con->connect(host, port);
		checkConnection(con);

		connectSubscriber(__STP_MV(con), listner, startThread);
	}

	// publisher
	{
		IpV6ClientConnection* con = new IpV6ClientConnection(); __STP(con);
		con->connect(host, port);
		checkConnection(con);

		connectPublisher(__STP_MV(con));
	}
}

void P2pHandshake::connectIpV4(const UnicodeString *host, int port) {
	connectIpV4(host, port, this, true);
}

void P2pHandshake::connectIpV4(const UnicodeString *host, int port, IPubsubCommandListner *listner, bool startThread) {
	// subscriber
	{
		IpV4ClientConnection* con = new IpV4ClientConnection(); __STP(con);
		con->connect(host, port);
		checkConnection(con);

		connectSubscriber(__STP_MV(con), listner, startThread);
	}

	// publisher
	{
		IpV4ClientConnection* con = new IpV4ClientConnection(); __STP(con);
		con->connect(host, port);
		checkConnection(con);

		connectPublisher(__STP_MV(con));
	}
}

IPubsubCommandExecutor* P2pHandshake::getExecutor() const noexcept {
	return this->executor;
}

void P2pHandshake::checkConnection(IClientSocket *con) {
	if(!con->isConnected()){
		throw new HandshakeConnectionException(__FILE__, __LINE__);
	}
}

void P2pHandshake::connectSubscriber(IClientSocket *con, IPubsubCommandListner *listner, bool startThread) {
	__STP(con);

	ConnectSubscriberCommand cmd(this->pubsubid);
	cmd.sendCommand(con);

	cmd.reveiveResponse(con);
	if(!cmd.isOk()){
		throw new HandshakeConnectionException(L"Bad server response.", __FILE__, __LINE__);
	}

	this->subscriber = new CommandSubscriber(__STP_MV(con), this->pubsubid, this->logger);
	if(startThread){
		this->subscriber->setListner(listner);
		this->subscriber->startSubscribe();
	}
}

void P2pHandshake::connectPublisher(IClientSocket *con) {
	__STP(con);

	ConnectPublisherCommand cmd(this->pubsubid);
	cmd.sendCommand(con);

	cmd.reveiveResponse(con);
	if(!cmd.isOk()){
		throw new HandshakeConnectionException(L"Bad server response.", __FILE__, __LINE__);
	}

	this->publisher = new CommandPublisher(__STP_MV(con), this->pubsubid);
}

AbstractCommandResponse* P2pHandshake::publishCommand(const AbstractPubSubCommand *cmd) {
	StackUnlocker __lock(this->publisherLock);

	if(this->publisher == nullptr || this->disabled){
		throw new HandshakeConnectionException(L"Publisher is closed.", __FILE__, __LINE__);
	}
	return this->publisher->publishCommand(cmd);
}

bool P2pHandshake::is2Delete() const noexcept {
	StackUnlocker __lock(this->useRefLock);

	return this->disabled && this->useRef == 0;
}

void P2pHandshake::setDisabled(bool bl) noexcept {
	StackUnlocker __lock(this->useRefLock);
	this->disabled = true;
}

void P2pHandshake::addRef() noexcept {
	StackUnlocker __lock(this->useRefLock);
	this->useRef++;
}

void P2pHandshake::decRef() noexcept {
	StackUnlocker __lock(this->useRefLock);
	this->useRef--;
}

int P2pHandshake::getRef() const noexcept {
	StackUnlocker __lock(this->useRefLock);
	return this->useRef;
}

void P2pHandshake::startSubscribe() {
	this->subscriber->setListner(this);
	this->subscriber->startSubscribe();
}

} /* namespace codablecash */
