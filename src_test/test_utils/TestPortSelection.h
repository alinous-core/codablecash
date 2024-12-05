/*
 * PortSelection.h
 *
 *  Created on: 2023/08/21
 *      Author: iizuka
 */

#ifndef TEST_UTILS_TESTPORTSELECTION_H_
#define TEST_UTILS_TESTPORTSELECTION_H_

#include "base/ArrayList.h"

namespace alinous {
class SysMutex;
}
using namespace alinous;

namespace codablecash {

class TestPort {
public:
	explicit TestPort(int port);
	~TestPort();

	int getPort() const noexcept {
		return this->port;
	}
	void inc() noexcept {
		this->ref++;
	}
	void dec() noexcept {
		this->ref--;
	}
	bool isUsed() const noexcept {
		return ref > 0;
	}

private:
	int port;
	int ref;
};

class TestPortSelection {
public:
	static constexpr int START_PORT = 20000;
	static constexpr int PORT_LENGTH = 200;

	TestPortSelection(int start, int length);
	virtual ~TestPortSelection();

	static TestPortSelection* getInstance();

	const TestPort* alloc() const noexcept;
	void release(int port);

	bool portUsed(int port);

private:
	ArrayList<TestPort> list;
	SysMutex* mutex;
};

class StackTestPortGetter {
public:
	StackTestPortGetter();
	~StackTestPortGetter();

	int allocPort() noexcept;

private:
	TestPortSelection* sel;

	ArrayList<const TestPort> list;
};

} /* namespace codablecash */

#endif /* TEST_UTILS_TESTPORTSELECTION_H_ */
