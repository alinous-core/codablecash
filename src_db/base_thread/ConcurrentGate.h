/*
 * ConcurrentGate.h
 *
 *  Created on: 2019/01/12
 *      Author: iizuka
 */

#ifndef BASE_THREAD_CONCURRENTGATE_H_
#define BASE_THREAD_CONCURRENTGATE_H_

#include "base_thread/SysMutex.h"
#include "base_thread/SynchronizedLock.h"

namespace alinous {

class ConcurrentGate {
public:
	ConcurrentGate() noexcept;
	virtual ~ConcurrentGate();

	void enter(const char *srcfile, int line) noexcept;
	void exit() noexcept;
	void close(const char *srcfile, int line) noexcept;
	void open() noexcept;

private:
	bool isOpened;
	int counter;
	int roomWaiter;

	SysMutex doorKeeperLock;
	SynchronizedLock stateLock;
	SynchronizedLock roomLock;
};

class StackReadLock {
public:
	StackReadLock(StackReadLock& inst) = delete;
	explicit StackReadLock(ConcurrentGate* gate, const char *srcfile, int line) noexcept;
	~StackReadLock();
private:
	ConcurrentGate* gate;
};

class StackWriteLock {
public:
	StackWriteLock(StackWriteLock& inst) = delete;
	explicit StackWriteLock(ConcurrentGate* gate, const char *srcfile, int line) noexcept;
	~StackWriteLock();

	ConcurrentGate* move() noexcept;

private:
	ConcurrentGate* gate;
};

} /* namespace alinous */

#endif /* BASE_THREAD_CONCURRENTGATE_H_ */
