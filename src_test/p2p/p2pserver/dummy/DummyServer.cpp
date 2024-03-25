/*
 * DummyServer.cpp
 *
 *  Created on: 2023/01/16
 *      Author: iizuka
 */

#include "DummyServer.h"

#include "ipconnect/IServerSocket.h"
#include "ipconnect/IpV6ServerConnection.h"

#include "osenv/funcs.h"

#include "p2pserver/P2pConnectionListeningThread.h"

namespace codablecash {

DummyServer::DummyServer() {
	this->thread = nullptr;
	this->srvSocket = nullptr;
	this->portSelf = 0;
}

DummyServer::~DummyServer() {
	this->thread->join();

	if(this->srvSocket != nullptr){
		delete this->srvSocket;
		this->srvSocket = nullptr;
	}

	delete this->thread;
}

void DummyServer::startIpV6Listening(const UnicodeString *host, int port, P2pConnectionListeningThread* thread) {
	this->portSelf = port;
	this->srvSocket = new IpV6ServerConnection();

	thread->setSocket(this->srvSocket);

	this->srvSocket->initAddress(host, port);
	this->srvSocket->bind();
	this->srvSocket->listen();

	this->thread = thread;
	this->thread->start();

	waitForStarted();
}

void DummyServer::waitForStarted() {
	do{
		Os::usleep(10000);
	}while(!this->thread->isContinueFlag());
}

} /* namespace codablecash */
