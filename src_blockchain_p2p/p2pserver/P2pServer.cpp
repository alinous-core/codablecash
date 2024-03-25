/*
 * P2pServer.cpp
 *
 *  Created on: 2022/08/23
 *      Author: iizuka
 */


#include "p2pserver/P2pServer.h"
#include "p2pserver/P2pConnectionListeningThread.h"
#include "p2pserver/NopConnectionCommand.h"
#include "p2pserver/P2pServerConnectionManager.h"

#include "ipconnect/IServerSocket.h"
#include "ipconnect/IpV6ServerConnection.h"
#include "ipconnect/IpV6ClientConnection.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "osenv/funcs.h"

#include "base_thread/SysMutex.h"
#include "base_thread/StackUnlocker.h"

#include "pubsub/CommandPublisher.h"
#include "pubsub/HandshakeConnectionException.h"
#include "pubsub/CommandSubscriber.h"

#include "pubsub/IPubsubCommandExecutor.h"

namespace codablecash {

P2pServer::P2pServer(ISystemLogger* logger, IPubsubCommandExecutor* executor) {
	this->srvSocket = nullptr;
	this->thread = nullptr;
	this->portSelf = 0;
	this->conManager = new P2pServerConnectionManager(logger, executor);
	this->pubsubWaitLock = new SysMutex();
	this->logger = logger;
}

P2pServer::~P2pServer() {
	closeServerSocket();
	dispose();

	delete this->pubsubWaitLock;

	this->logger = nullptr;
}

void P2pServer::dispose() noexcept {
	if(this->conManager != nullptr){
		this->conManager->dispose();
		delete this->conManager;
		this->conManager = nullptr;
	}
}

void P2pServer::startIpV6Listening(const UnicodeString *host, int port) {
	this->portSelf = port;
	this->srvSocket = new IpV6ServerConnection();

	this->srvSocket->initAddress(host, port);
	this->srvSocket->bind();
	this->srvSocket->listen();

	this->thread = new P2pConnectionListeningThread(this->srvSocket, this);
	this->thread->start();

	waitForStarted();
}

void P2pServer::stopListning() {
	if(this->thread != nullptr){
		this->thread->setContinueFlag(false);

		IpV6ClientConnection con;
		UnicodeString strLocal(L"::1");
		con.connect(&strLocal, this->portSelf);

		NopConnectionCommand* cmd = new NopConnectionCommand(); __STP(cmd);
		cmd->sendCommand(&con);

		this->thread->join();
		delete this->thread;
		this->thread = nullptr;

		con.close();
	}
}

void P2pServer::close() noexcept {
	closeServerSocket();
}

void P2pServer::closeServerSocket() noexcept {
	if(this->srvSocket != nullptr){
		delete this->srvSocket;
		this->srvSocket = nullptr;
	}
}

void P2pServer::waitForStarted() {
	while(!this->thread->isContinueFlag()) Os::usleep(1000);
}

void P2pServer::registerPublisher(CommandPublisher *publisher) {
	StackUnlocker __lock(this->pubsubWaitLock);

	const PubSubId* pubsubid = publisher->getPubsubId();

	this->conManager->removeDisabledHandshake();
	if(this->conManager->handShakeExists(pubsubid)){
		publisher->moveSocket();
		delete publisher;
		throw new HandshakeConnectionException(L"Connection id is already used by a handshake.", __FILE__, __LINE__);
	}

	CommandPublisher* p = this->conManager->getPublisher(pubsubid);
	if(p != nullptr){
		publisher->moveSocket();
		delete publisher;
		throw new HandshakeConnectionException(L"Connection id is already used.", __FILE__, __LINE__);
	}

	this->conManager->registerPublisher(publisher);
}

void P2pServer::newHandShake(CommandSubscriber *subscriber) {
	StackUnlocker __lock(this->pubsubWaitLock);

	const PubSubId* pubsubId = subscriber->getPubsubId();
	CommandPublisher* p = this->conManager->getPublisher(pubsubId, true);
	if(p == nullptr){
		subscriber->moveSocket();
		delete subscriber;
		throw new HandshakeConnectionException(L"Connection id is already used.", __FILE__, __LINE__);
	}

	this->conManager->newHandshake(p, subscriber);
}

void P2pServer::registerHandshake(P2pHandshake *handshake) {
	this->conManager->registerHandshake(handshake);
}

void P2pServer::addConnectionListener(IPubsubConnectionListener *listner) noexcept {
	this->conManager->addConnectionListener(listner);
}

} /* namespace codablecash */
