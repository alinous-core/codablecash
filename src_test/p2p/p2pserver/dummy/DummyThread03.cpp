/*
 * DummyThread03.cpp
 *
 *  Created on: 2023/01/16
 *      Author: iizuka
 */

#include "DummyThread03.h"

#include "base/StackRelease.h"

#include "ipconnect/IClientSocket.h"
#include "ipconnect/IServerSocket.h"

#include "base_io/ByteBuffer.h"

#include <cstdint>
namespace codablecash {

DummyThread03::DummyThread03()
		: P2pConnectionListeningThread(nullptr, nullptr, nullptr){

}

DummyThread03::~DummyThread03() {

}

void DummyThread03::process() noexcept {
	setContinueFlag(true);

	IClientSocket* client = this->srvSocket->accept(); __STP(client);

	const char* str = "NG. wrong connection id.";
	int size = ::strlen(str) + 1;
	int capacity = sizeof(uint32_t) + size;

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(capacity, true); __STP(buff);
	buff->putInt(size);
	buff->put((const uint8_t*)str, size);

	buff->position(0);

	int n = client->write((const char*)buff->array(), buff->limit());

	setContinueFlag(false);
}

} /* namespace codablecash */
