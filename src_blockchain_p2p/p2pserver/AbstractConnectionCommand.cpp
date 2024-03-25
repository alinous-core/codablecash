/*
 * AbstractConnectionCommand.cpp
 *
 *  Created on: 2022/08/23
 *      Author: iizuka
 */

#include "p2pserver/AbstractConnectionCommand.h"

#include "p2pserver/NopConnectionCommand.h"
#include "p2pserver/ConnectSubscriberCommand.h"
#include "p2pserver/ConnectPublisherCommand.h"
#include "p2pserver/WrongPubsubConnectionCommandException.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"

#include "ipconnect/IClientSocket.h"

#include "pubsub/HandshakeConnectionException.h"

namespace codablecash {

AbstractConnectionCommand::AbstractConnectionCommand(uint8_t type) {
	this->type = type;
	this->responseBinary = nullptr;
}

AbstractConnectionCommand::~AbstractConnectionCommand() {
	delete this->responseBinary;
}

AbstractConnectionCommand* AbstractConnectionCommand::createFromBinary(ByteBuffer *buff) {
	AbstractConnectionCommand* ret = nullptr;

	buff->position(0);
	uint8_t type = buff->get();
	switch(type){
	case AbstractConnectionCommand::TYPE_NOP:
		ret = new NopConnectionCommand();
		break;
	case AbstractConnectionCommand::TYPE_CONNECT_PUBLISHER:
		ret = new ConnectPublisherCommand();
		break;
	case AbstractConnectionCommand::TYPE_CONNECT_SUBSCRIBER:
		ret = new ConnectSubscriberCommand();
		break;
	default:
		throw new WrongPubsubConnectionCommandException(__FILE__, __LINE__);
	}

	ret->fromBinary(buff);

	return ret;
}

void AbstractConnectionCommand::sendCommand(IClientSocket *client) {
	int size = binarySize();
	int length = size + sizeof(uint32_t);

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(length, true); __STP(buff);
	buff->putInt(size);

	toBinary(buff);

	buff->position(0);
	const uint8_t* ar = buff->array();

	client->write((const char*)ar, length);
}

void AbstractConnectionCommand::reveiveResponse(IClientSocket *client) {
	char tmp[4]{};
	int n = client->read(tmp, sizeof(tmp));
	if(n < 4){
		throw new HandshakeConnectionException(L"wrong response.", __FILE__, __LINE__);
	}

	ByteBuffer* b = ByteBuffer::wrapWithEndian((const uint8_t*)tmp, 4, true); __STP(b);
	int estimatedSize = b->getInt();
	if(estimatedSize > 10000 || estimatedSize < 0){
		throw new HandshakeConnectionException(L"too big response.", __FILE__, __LINE__);
	}

	this->responseBinary = readAll(client, estimatedSize);
	this->responseBinary->position(0);
}

bool AbstractConnectionCommand::isOk() {
	this->responseBinary->position(0);
	const char* str = (const char*)this->responseBinary->array();

	int diff = ::strcasecmp("ok", str);
	return diff == 0;
}

ByteBuffer* AbstractConnectionCommand::readAll(IClientSocket *client, int size) {
	char* b = new char[size]{};
	StackArrayRelease<char> st_b(b);

	int length = client->read(b, size);

	ByteBuffer* buff = ByteBuffer::wrapWithEndian((const uint8_t*)b, length, true);
	return buff;
}

void AbstractConnectionCommand::sendResponseString(IClientSocket *client, const char *str, int size) {
	// write size
	ByteBuffer* intb = ByteBuffer::allocateWithEndian(sizeof(uint32_t), true); __STP(intb);
	intb->putInt(size);

	intb->position(0);
	const char* intar = (const char*)intb->array();
	client->write(intar, sizeof(uint32_t));

	// write string
	client->write(str, size);
}

void AbstractConnectionCommand::sendOkString(IClientSocket *client) {
	const char* okstr = "ok";
	sendResponseString(client, okstr, 3);
}

} /* namespace codablecash */
