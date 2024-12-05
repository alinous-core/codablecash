/**
 * @file	LockingRecord.h
 * @date	2024/04/22
 * @author	iizuka 
 * @version 0.0.1
 */
#ifndef BASE_THREAD_LOCKCHECK_LOCKINGRECORD_H_
#define BASE_THREAD_LOCKCHECK_LOCKINGRECORD_H_

namespace alinous {

class ILock;

class LockingRecord {
public:
	LockingRecord(const ILock* lock, const char* srcfile, int line);
	virtual ~LockingRecord();

	bool equals(const LockingRecord* other) const noexcept;

	const ILock* getLock() const noexcept {
		return this->lock;
	}

private:
	const char* srcfile;
	int line;
	const ILock* lock;
};

} /* namespace alinous */

#endif /* BASE_THREAD_LOCKCHECK_LOCKINGRECORD_H_ */
