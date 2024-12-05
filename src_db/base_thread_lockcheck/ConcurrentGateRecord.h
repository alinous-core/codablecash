/**
 * @file	ConcurrentGateRecord.h
 * @date	2024/04/22
 * @author	iizuka 
 * @version 0.0.1
 */
#ifndef BASE_THREAD_LOCKCHECK_CONCURRENTGATERECORD_H_
#define BASE_THREAD_LOCKCHECK_CONCURRENTGATERECORD_H_

namespace alinous {

class ConcurrentGate;

class ConcurrentGateRecord {
public:
	ConcurrentGateRecord(const ConcurrentGate* gate, const char* srcfile, int line);
	virtual ~ConcurrentGateRecord();

	bool equals(const ConcurrentGateRecord* other) const noexcept;

	const ConcurrentGate* getGate() const noexcept {
		return this->gate;
	}

private:
	const char* srcfile;
	int line;
	const ConcurrentGate* gate;
};

} /* namespace alinous */

#endif /* BASE_THREAD_LOCKCHECK_CONCURRENTGATERECORD_H_ */
