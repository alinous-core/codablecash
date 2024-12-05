/**
 * @file	ThreadSpecificLocks.cpp
 * @date	2024/04/22
 * @author	iizuka 
 * @version 0.0.1
 */
#include "base_thread_lockcheck/ThreadSpecificLocks.h"
#include "base_thread_lockcheck/LockingRecord.h"
#include "base_thread_lockcheck/ConcurrentGateRecord.h"


namespace alinous {


ThreadSpecificLocks::ThreadSpecificLocks() {

}

ThreadSpecificLocks::~ThreadSpecificLocks() {
	assert(this->list.isEmpty());
	assert(this->gatelist.isEmpty());
}

void ThreadSpecificLocks::addLock(LockingRecord *record) {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		LockingRecord* rec = this->list.get(i);

		assert(!record->equals(rec));
	}

	this->list.addElement(record);
}

void ThreadSpecificLocks::removeLock(const ILock* lock) {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		LockingRecord* rec = this->list.get(i);
		const ILock* l = rec->getLock();

		if(l == lock){
			this->list.removeByObj(rec);
			delete rec;
		}
	}
}

bool ThreadSpecificLocks::isEmpty() const noexcept {
	return this->list.isEmpty() && this->gatelist.isEmpty();
}

void ThreadSpecificLocks::addGate(ConcurrentGateRecord *record) {
	int maxLoop = this->gatelist.size();
	for(int i = 0; i != maxLoop; ++i){
		ConcurrentGateRecord* rec = this->gatelist.get(i);

		assert(!record->equals(rec));
	}

	this->gatelist.addElement(record);
}

void ThreadSpecificLocks::removeGate(const ConcurrentGate *gate) {
	int maxLoop = this->gatelist.size();
	for(int i = 0; i != maxLoop; ++i){
		ConcurrentGateRecord* rec = this->gatelist.get(i);
		const ConcurrentGate* g = rec->getGate();

		if(g == gate){
			this->gatelist.removeByObj(rec);
			delete rec;
		}
	}
}

}
