/*
 * DummyThread01.cpp
 *
 *  Created on: 2023/01/16
 *      Author: iizuka
 */

#include "DummyThread01.h"

#include "ipconnect/IClientSocket.h"
#include "ipconnect/IServerSocket.h"

#include "base/StackRelease.h"

#include <cstdint>

namespace codablecash {

DummyThread01::DummyThread01()
		: P2pConnectionListeningThread(nullptr, nullptr, nullptr) {

}

DummyThread01::~DummyThread01() {

}

void DummyThread01::process() noexcept {
	setContinueFlag(true);

	IClientSocket* client = this->srvSocket->accept(); __STP(client);

	int size = 1024 * 4;
	char* buff = new char[size];
	StackArrayRelease<char> __st_buff(buff);

	client->read(buff, size);

	uint8_t ch = 0;
	int n = client->write((const char*)&ch, 1);

	setContinueFlag(false);
}

} /* namespace codablecash */
