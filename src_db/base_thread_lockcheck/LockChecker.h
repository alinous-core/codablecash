/**
 * @file	LockChecker.h
 * @date	2024/04/22
 * @author	iizuka 
 * @version 0.0.1
 */
#ifndef BASE_THREAD_LOCKCHECK_LOCKCHECKER_H_
#define BASE_THREAD_LOCKCHECK_LOCKCHECKER_H_

#include <pthread.h>

#include "osenv/funcs.h"
#include "base/ArrayList.h"

namespace alinous {

typedef struct __threadKeySet {
	THREAD_ID id;
	THREAD_KEY key;
} ThreadKeySet;

class ILock;
class ThreadSpecificLocks;
class ConcurrentGate;

class LockChecker {
public:
	LockChecker();
	virtual ~LockChecker();

	static LockChecker* getInstance();

	void checklock(const ILock* lock, const char* srcfile, int line);
	void removelock(const ILock* lock);

	void checkgate(const ConcurrentGate* gate, const char* srcfile, int line);
	void removegate(const ConcurrentGate* gate);

	ThreadKeySet* getKey();
	void removeKey(THREAD_ID tid);
	ThreadSpecificLocks* getThreadSpecificLocks();

private:
	ArrayList<ThreadKeySet> list;
	pthread_mutex_t mutex;
};

}

#endif /* BASE_THREAD_LOCKCHECK_LOCKCHECKER_H_ */
