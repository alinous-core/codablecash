/*
 * DummyThread02.cpp
 *
 *  Created on: 2023/01/16
 *      Author: iizuka
 */

#include "DummyThread02.h"

#include "ipconnect/IClientSocket.h"
#include "ipconnect/IServerSocket.h"

#include "base/StackRelease.h"

#include <cstdint>

namespace codablecash {

DummyThread02::DummyThread02()
		: P2pConnectionListeningThread(nullptr, nullptr, nullptr) {
}

DummyThread02::~DummyThread02() {
}

void DummyThread02::process() noexcept {
	setContinueFlag(true);

	IClientSocket* client = this->srvSocket->accept(); __STP(client);

	int size = 1024 * 4;
	char* buff = new char[size];
	StackArrayRelease<char> __st_buff(buff);

	client->read(buff, size);

	uint32_t ch = 10000000;
	int n = client->write((const char*)&ch, sizeof(ch));

	setContinueFlag(false);
}

} /* namespace codablecash */
