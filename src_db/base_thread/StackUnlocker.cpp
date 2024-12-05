/*
 * StackUnlocker.cpp
 *
 *  Created on: 2018/04/27
 *      Author: iizuka
 */

#include "base_thread/StackUnlocker.h"
#include "base_thread/ILock.h"

namespace alinous {

StackUnlocker::StackUnlocker(ILock* mutex, const char *srcfile, int line) noexcept {
	this->mutex = mutex;
	mutex->lock(srcfile, line);
}

StackUnlocker::~StackUnlocker() noexcept {
	mutex->unlock();
}

} /* namespace alinous */
