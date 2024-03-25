/*
 * ConnectPublisherCommand.cpp
 *
 *  Created on: 2023/01/10
 *      Author: iizuka
 */

#include "p2pserver/ConnectPublisherCommand.h"

#include "p2pserver/P2pConnectionAcceptThread.h"
#include "p2pserver/P2pServer.h"

#include "pubsub/PubSubId.h"
#include "pubsub/CommandSubscriber.h"
#include "pubsub/HandshakeConnectionException.h"

#include "base_io/ByteBuffer.h"

#include "ipconnect/IClientSocket.h"

#include <cstring>

namespace codablecash {

ConnectPublisherCommand::ConnectPublisherCommand() : AbstractConnectionCommand(AbstractConnectionCommand::TYPE_CONNECT_PUBLISHER) {
	this->pubsubId = nullptr;
}

ConnectPublisherCommand::ConnectPublisherCommand(const PubSubId *pubsubId) : AbstractConnectionCommand(AbstractConnectionCommand::TYPE_CONNECT_PUBLISHER) {
	this->pubsubId = new PubSubId(*pubsubId);
}

ConnectPublisherCommand::~ConnectPublisherCommand() {
	delete this->pubsubId;
}

void ConnectPublisherCommand::fromBinary(ByteBuffer *buff) {
	this->pubsubId = PubSubId::fromBinary(buff);
}

int ConnectPublisherCommand::binarySize() {
	int total = sizeof(uint8_t);

	total += this->pubsubId->binarySize();

	return total;
}

void ConnectPublisherCommand::toBinary(ByteBuffer *buff) {
	buff->put(this->type);

	this->pubsubId->toBinary(buff);
}

void ConnectPublisherCommand::process(P2pConnectionAcceptThread *acceptThread) {
	IClientSocket* client = acceptThread->delicateConnection();

	P2pServer* p2pServer = acceptThread->getP2pServer();
	ISystemLogger* logger = p2pServer->getLogger();

	CommandSubscriber* subscriber = new CommandSubscriber(client, this->pubsubId, logger);

	try{
		p2pServer->newHandShake(subscriber);
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
