/*
 * P2pConnectionAcceptThread.cpp
 *
 *  Created on: 2022/08/26
 *      Author: iizuka
 */

#include "p2pserver/P2pConnectionAcceptThread.h"

#include "ipconnect/IClientSocket.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"
#include "base/Exception.h"

#include "p2pserver/AbstractConnectionCommand.h"

#include "pubsub/HandshakeConnectionException.h"


namespace codablecash {

P2pConnectionAcceptThread::P2pConnectionAcceptThread(IClientSocket* client, P2pServer* p2pServer, const UnicodeString* name)
		: AbstractThreadRunner(name) {
	this->client = client;
	this->p2pServer = p2pServer;
	this->done = false;
}

P2pConnectionAcceptThread::~P2pConnectionAcceptThread() {
	delete this->client;
	this->client = nullptr;
	this->p2pServer = nullptr;
}

void P2pConnectionAcceptThread::process() noexcept {

	// check by command type
	char tmp[4]{};
	int n = client->read(tmp, sizeof(tmp));
	if(n < 4){
		this->done = true;
		return;
	}

	try{
		ByteBuffer* b = ByteBuffer::wrapWithEndian((const uint8_t*)tmp, 4, true); __STP(b);
		int estimatedSize = b->getInt();
		if(estimatedSize > 10000 || estimatedSize < 0){
			throw new HandshakeConnectionException(L"too big response.", __FILE__, __LINE__);
		}

		ByteBuffer* buff = readAll(client, estimatedSize); __STP(buff);

		AbstractConnectionCommand* cmd = AbstractConnectionCommand::createFromBinary(buff); __STP(cmd);

		// handle connection command
		cmd->process(this);
	}
	catch(Exception* e){
		delete e;
	}

	this->done = true;
}

IClientSocket* P2pConnectionAcceptThread::delicateConnection() {
	IClientSocket* ret = this->client;
	this->client = nullptr;

	return ret;
}

ByteBuffer* P2pConnectionAcceptThread::readAll(IClientSocket *client, int size) {
	char* b = new char[size]{};
	StackArrayRelease<char> st_b(b);

	int length = client->read(b, size);

	ByteBuffer* buff = ByteBuffer::wrapWithEndian((const uint8_t*)b, length, true);
	return buff;
}

} /* namespace codablecash */
