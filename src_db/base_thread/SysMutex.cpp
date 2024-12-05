/*
 * SysMutex.cpp
 *
 *  Created on: 2018/04/13
 *      Author: iizuka
 */

#include "debug/debugMacros.h"

#include "SysMutex.h"

#include "base_thread_lockcheck/LockChecker.h"
namespace alinous {

SysMutex::SysMutex() noexcept : ILock() {
	int ret = pthread_mutex_init(&mutex, nullptr);
	assert(ret == 0);

	this->count = 0;
}

SysMutex::~SysMutex() noexcept {
	int ret = pthread_mutex_destroy(&mutex);
	assert(ret == 0);
}

void SysMutex::lock(const char *srcfile, int line) noexcept {
#ifdef __DEBUG__
	LockChecker* checker = LockChecker::getInstance();
	checker->checklock(this, srcfile, line);
#endif

	int ret = pthread_mutex_lock(&mutex);
	assert(ret == 0);

	this->count++;
}


void SysMutex::unlock() noexcept {
#ifdef __DEBUG__
	LockChecker* checker = LockChecker::getInstance();
	checker->removelock(this);
#endif

	this->count--;
	int ret = pthread_mutex_unlock(&mutex);
	assert(ret == 0);
}


} /* namespace codablecash */
