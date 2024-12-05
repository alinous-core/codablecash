/**
 * @file	LockingRecord.cpp
 * @date	2024/04/22
 * @author	iizuka 
 * @version 0.0.1
 */
#include "base_thread_lockcheck/LockingRecord.h"

namespace alinous {

LockingRecord::LockingRecord(const ILock* lock, const char* srcfile, int line) {
	this->lock = lock;
	this->srcfile = srcfile;
	this->line = line;
}

LockingRecord::~LockingRecord() {

}

bool LockingRecord::equals(const LockingRecord *other) const noexcept {
	return this->lock == other->lock;
}

} /* namespace alinous */
