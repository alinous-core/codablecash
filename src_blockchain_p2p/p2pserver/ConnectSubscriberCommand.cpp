/*
 * ConnectSubscriberCommand.cpp
 *
 *  Created on: 2023/01/10
 *      Author: iizuka
 */

#include "p2pserver/ConnectSubscriberCommand.h"

#include "p2pserver/P2pConnectionAcceptThread.h"
#include "p2pserver/P2pServer.h"

#include "base_io/ByteBuffer.h"

#include "pubsub/PubSubId.h"
#include "pubsub/CommandPublisher.h"

#include "pubsub/HandshakeConnectionException.h"

#include <cstring>

#include "ipconnect/IClientSocket.h"

namespace codablecash {

ConnectSubscriberCommand::ConnectSubscriberCommand() : AbstractConnectionCommand(AbstractConnectionCommand::TYPE_CONNECT_SUBSCRIBER) {
	this->pubsubId = nullptr;
}

ConnectSubscriberCommand::ConnectSubscriberCommand(const PubSubId* pubsubId) : AbstractConnectionCommand(AbstractConnectionCommand::TYPE_CONNECT_SUBSCRIBER) {
	this->pubsubId = new PubSubId(*pubsubId);
}

ConnectSubscriberCommand::~ConnectSubscriberCommand() {
	delete this->pubsubId;
}

void ConnectSubscriberCommand::fromBinary(ByteBuffer *buff) {
	this->pubsubId = PubSubId::fromBinary(buff);
}

int ConnectSubscriberCommand::binarySize() {
	int total = sizeof(uint8_t);

	total += this->pubsubId->binarySize();

	return total;
}

void ConnectSubscriberCommand::toBinary(ByteBuffer *buff) {
	buff->put(this->type);

	this->pubsubId->toBinary(buff);
}

void ConnectSubscriberCommand::process(P2pConnectionAcceptThread *acceptThread) {
	IClientSocket* client = acceptThread->delicateConnection();
	P2pServer* p2pServer = acceptThread->getP2pServer();

	CommandPublisher* publisher = new CommandPublisher(client, this->pubsubId);
	try{
		p2pServer->registerPublisher(publisher);
	}
	catch(HandshakeConnectionException* e){
		const char* str = "NG. wrong connection id.";
		int size = ::strlen(str) + 1;
		sendResponseString(client, str, size);

		client->close();
		delete client;
		delete e;
		return;
	}

	sendOkString(client);
}

} /* namespace codablecash */
