/**
 * @file	LockChecker.cpp
 * @date	2024/04/22
 * @author	iizuka 
 * @version 0.0.1
 */
#include "base_thread_lockcheck/LockChecker.h"
#include "base_thread_lockcheck/LockingRecord.h"
#include "base_thread_lockcheck/ThreadSpecificLocks.h"
#include "base_thread_lockcheck/ConcurrentGateRecord.h"

#include "base/StackRelease.h"


namespace alinous {

LockChecker::LockChecker() {
	pthread_mutex_init(&mutex, nullptr);
}

LockChecker::~LockChecker() {
/*	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		ThreadKeySet* key = list.get(i);
		Os::threadKeyDelete(key->key);

		delete key;
	}*/
	assert(this->list.size() == 0);

	pthread_mutex_destroy(&mutex);
}

LockChecker* LockChecker::getInstance() {
	static LockChecker inst;

	return &inst;
}

ThreadSpecificLocks* LockChecker::getThreadSpecificLocks() {
	ThreadKeySet* pkey = getKey();

	if(pkey == nullptr){
		pkey = new ThreadKeySet;
		pkey->id = Os::getCurrentThreadId();
		int ret = Os::threadKeyCreate(&pkey->key);

		pthread_mutex_lock(&mutex);
		this->list.addElement(pkey);
		pthread_mutex_unlock(&mutex);
	}

	void* ptr = Os::threadGetSpecific(pkey->key);
	ThreadSpecificLocks* locks = static_cast<ThreadSpecificLocks*>(ptr);

	if(locks == nullptr){
		locks = new ThreadSpecificLocks();
		Os::threadSetSpecific(pkey->key, locks);
	}

	return locks;
}

ThreadKeySet* LockChecker::getKey() {
	THREAD_ID tid = Os::getCurrentThreadId();

	ThreadKeySet* ret = nullptr;

	pthread_mutex_lock(&mutex);

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		ThreadKeySet* key = list.get(i);

		if(key->id == tid){
			ret = key;
			break;
		}
	}

	pthread_mutex_unlock(&mutex);

	return ret;
}

void LockChecker::checklock(const ILock *lock, const char *srcfile, int line) {
	LockingRecord* record = new LockingRecord(lock, srcfile, line);

	ThreadSpecificLocks* locks = getThreadSpecificLocks();
	locks->addLock(record);
}

void LockChecker::removelock(const ILock *lock) {
	ThreadSpecificLocks* locks = getThreadSpecificLocks();

	locks->removeLock(lock);

	if(locks->isEmpty()){
		THREAD_ID tid = Os::getCurrentThreadId();
		removeKey(tid);
		delete locks;
	}
}

void LockChecker::removeKey(THREAD_ID tid) {
	pthread_mutex_lock(&mutex);

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		ThreadKeySet* key = list.get(i);

		if(key->id == tid){
			this->list.removeByObj(key);
			Os::threadKeyDelete(key->key);
			delete key;

			break;
		}
	}

	pthread_mutex_unlock(&mutex);
}

void LockChecker::checkgate(const ConcurrentGate *gate, const char *srcfile, int line) {
	ConcurrentGateRecord *record = new ConcurrentGateRecord(gate, srcfile, line);

	ThreadSpecificLocks* locks = getThreadSpecificLocks();
	locks->addGate(record);
}

void LockChecker::removegate(const ConcurrentGate *gate) {
	ThreadSpecificLocks* locks = getThreadSpecificLocks();
	locks->removeGate(gate);

	if(locks->isEmpty()){
		THREAD_ID tid = Os::getCurrentThreadId();
		removeKey(tid);
		delete locks;
	}
}

}

