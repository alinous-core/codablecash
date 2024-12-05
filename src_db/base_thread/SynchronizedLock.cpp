/*
 * SynchronizedLock.cpp
 *
 *  Created on: 2018/04/28
 *      Author: iizuka
 */

#include "base_thread/SynchronizedLock.h"

#include "base_thread_lockcheck/LockChecker.h"

#include "debug/debugMacros.h"

namespace alinous {

SynchronizedLock::SynchronizedLock() : ILock() {
	this->cond = new LockCondition();
	int ret = ::pthread_mutex_init(&mutex, NULL);
	assert(ret == 0);
}

SynchronizedLock::~SynchronizedLock() {
	delete this->cond, this->cond = nullptr;

	int ret = ::pthread_mutex_destroy(&mutex);
	assert(ret == 0);
}

void SynchronizedLock::lock(const char *srcfile, int line) {
#ifdef __DEBUG__
	LockChecker* checker = LockChecker::getInstance();
	checker->checklock(this, srcfile, line);
#endif

	int ret = ::pthread_mutex_lock(&mutex);
	assert(ret == 0);
}

void SynchronizedLock::unlock() {
#ifdef __DEBUG__
	LockChecker* checker = LockChecker::getInstance();
	checker->removelock(this);
#endif

	int ret = ::pthread_mutex_unlock(&mutex);
	assert(ret == 0);
}

void SynchronizedLock::wait() {
	int ret = ::pthread_cond_wait(&this->cond->cond, &mutex);
	assert(ret == 0);
}

void SynchronizedLock::notifyAll() {
	int ret = ::pthread_cond_broadcast(&this->cond->cond);
	assert(ret == 0);
}

void SynchronizedLock::notify() {
	::pthread_cond_signal(&this->cond->cond);
}

uint64_t SynchronizedLock::getObjectSize() noexcept {
	uint64_t size = sizeof(LockCondition*) + sizeof(pthread_cond_t);
	return size;
}

/*
int SynchronizedLock::tryLock() {
	int ret = ::pthread_mutex_trylock(&mutex);
	return ret;
}*/

} /* namespace alinous */
