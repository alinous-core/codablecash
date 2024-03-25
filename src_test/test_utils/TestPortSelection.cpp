/*
 * PortSelection.cpp
 *
 *  Created on: 2023/08/21
 *      Author: iizuka
 */

#include "TestPortSelection.h"

#include "ipconnect/IpV6ClientConnection.h"

#include "base/StackRelease.h"

#include "base/UnicodeString.h"

#include "base_thread/SysMutex.h"

#include "base_thread/StackUnlocker.h"
namespace codablecash {

TestPort::TestPort(int port) {
	this->port = port;
	this->ref = 0;
}

TestPort::~TestPort() {
}

TestPortSelection::TestPortSelection(int start, int length) : list(length) {
	this->mutex = new SysMutex();

	for(int i = 0; i != length; ++i){
		int port = start + i;

		bool used = portUsed(port);
		if(!used){
			this->list.addElement(new TestPort(port));
		}
	}
}

TestPortSelection::~TestPortSelection() {
	this->list.deleteElements();
	delete this->mutex;
}

TestPortSelection* TestPortSelection::getInstance() {
	static TestPortSelection inst(START_PORT, PORT_LENGTH);

	return &inst;
}

const TestPort* TestPortSelection::alloc() const noexcept {
	StackUnlocker lock(this->mutex);

	TestPort* ret = nullptr;
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		TestPort* p = this->list.get(i);

		if(!p->isUsed()){
			p->inc();
			ret = p;
			break;
		}
	}

	return ret;
}

void TestPortSelection::release(int port) {
	StackUnlocker lock(this->mutex);

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		TestPort* p = this->list.get(i);

		if(p->getPort() == port){
			p->dec();
			break;
		}
	}
}

bool TestPortSelection::portUsed(int port) {
	IpV6ClientConnection* con = new IpV6ClientConnection(); __STP(con);

	UnicodeString strLocal(L"::1");
	con->connect(&strLocal, port);

	bool ret = con->isConnected();
	con->close();

	return ret;
}

StackTestPortGetter::StackTestPortGetter() {
	this->sel = TestPortSelection::getInstance();
}

StackTestPortGetter::~StackTestPortGetter() {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		const TestPort* p = this->list.get(i);
		this->sel->release(p->getPort());
	}
}

int StackTestPortGetter::allocPort() noexcept {
	const TestPort* port = this->sel->alloc();
	this->list.addElement(port);

	return port->getPort();
}

} /* namespace codablecash */
