/**
 * @file	ThreadSpecificLocks.h
 * @date	2024/04/22
 * @author	iizuka 
 * @version 0.0.1
 */
#ifndef BASE_THREAD_LOCKCHECK_THREADSPECIFICLOCKS_H_
#define BASE_THREAD_LOCKCHECK_THREADSPECIFICLOCKS_H_

#include "base/ArrayList.h"


namespace alinous {

class ILock;
class LockingRecord;
class ConcurrentGateRecord;
class ConcurrentGate;

class ThreadSpecificLocks {
public:
	ThreadSpecificLocks();
	virtual ~ThreadSpecificLocks();

	void addLock(LockingRecord* record);
	void removeLock(const ILock* lock);

	void addGate(ConcurrentGateRecord* record);
	void removeGate(const ConcurrentGate* gate);

	bool isEmpty() const noexcept;

private:
	ArrayList<LockingRecord> list;
	ArrayList<ConcurrentGateRecord> gatelist;
};

}

#endif /* BASE_THREAD_LOCKCHECK_THREADSPECIFICLOCKS_H_ */
