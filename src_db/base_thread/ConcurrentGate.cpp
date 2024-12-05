/*
 * ConcurrentGate.cpp
 *
 *  Created on: 2019/01/12
 *      Author: iizuka
 */

#include "base_thread/ConcurrentGate.h"
#include "base_thread/StackUnlocker.h"

#include "base_thread_lockcheck/LockChecker.h"

namespace alinous {

ConcurrentGate::ConcurrentGate() noexcept {
	this->isOpened = true;
	this->counter = 0;
	this->roomWaiter = 0;
}

ConcurrentGate::~ConcurrentGate() {
}

void ConcurrentGate::enter(const char *srcfile, int line) noexcept {
#ifdef __DEBUG__
	LockChecker* checker = LockChecker::getInstance();
	checker->checkgate(this, srcfile, line);
#endif

	{
		StackUnlocker __st_lock(&this->stateLock, __FILE__, __LINE__);
		if(this->isOpened){
			this->counter++;
		}
		else{
			{
				StackUnlocker __st_lock2(&this->roomLock, __FILE__, __LINE__);
				this->roomWaiter++;
			}

			while(!this->isOpened){
				this->stateLock.wait();
			}

			/* ---------------------
						ROOM
			 ---------------------- */
			{
				StackUnlocker __st_lock2(&this->roomLock, __FILE__, __LINE__);

				this->roomWaiter--;
				if(this->roomWaiter == 0){
					this->roomLock.notify();
				}
			}
			this->counter++;
		}
	}
}

void ConcurrentGate::exit() noexcept {
#ifdef __DEBUG__
	LockChecker* checker = LockChecker::getInstance();
	checker->removegate(this);
#endif

	{
		StackUnlocker __st_lock(&this->stateLock, __FILE__, __LINE__);
		this->counter--;
		if(this->counter == 0){
			this->stateLock.notifyAll();
		}
	}
}

void ConcurrentGate::open() noexcept {
#ifdef __DEBUG__
	LockChecker* checker = LockChecker::getInstance();
	checker->removegate(this);
#endif
	{
		StackUnlocker __st_lock(&this->stateLock, __FILE__, __LINE__);

		this->isOpened = true;
		this->stateLock.notifyAll();
	}

	{
		StackUnlocker __st_lock(&this->roomLock, __FILE__, __LINE__);

		if(this->roomWaiter > 0){
			this->roomLock.wait();
		}
	}

	this->doorKeeperLock.unlock();
}

void ConcurrentGate::close(const char *srcfile, int line) noexcept {
#ifdef __DEBUG__
	LockChecker* checker = LockChecker::getInstance();
	checker->checkgate(this, srcfile, line);
#endif

	this->doorKeeperLock.lock(__FILE__, __LINE__);

	{
		StackUnlocker __st_lock(&this->stateLock, __FILE__, __LINE__);

		this->isOpened = false; // close the gate
		while(this->counter > 0){
			//out(" [ Gate :" + Thread.currentThread().getId() + "] wait for reader threads ends");
			this->stateLock.wait();
		}
	}
}


StackReadLock::StackReadLock(ConcurrentGate* gate, const char *srcfile, int line) noexcept {
	this->gate = gate;
	gate->enter(srcfile, line);
}

StackReadLock::~StackReadLock() {
	gate->exit();
}

StackWriteLock::StackWriteLock(ConcurrentGate* gate, const char *srcfile, int line) noexcept {
	this->gate = gate;
	gate->close(srcfile, line);
}

StackWriteLock::~StackWriteLock() {
	if(this->gate != nullptr){
		this->gate->open();
	}
}

ConcurrentGate* StackWriteLock::move() noexcept {
	ConcurrentGate* g = this->gate;
	this->gate = nullptr;

	return g;
}

} /* namespace alinous */
