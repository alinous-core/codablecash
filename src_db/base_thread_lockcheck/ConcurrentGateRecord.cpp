/**
 * @file	ConcurrentGateRecord.cpp
 * @date	2024/04/22
 * @author	iizuka 
 * @version 0.0.1
 */
#include "base_thread_lockcheck/ConcurrentGateRecord.h"

namespace alinous {

ConcurrentGateRecord::ConcurrentGateRecord(const ConcurrentGate* gate, const char* srcfile, int line) {
	this->gate = gate;
	this->srcfile = srcfile;
	this->line = line;
}

ConcurrentGateRecord::~ConcurrentGateRecord() {

}

bool ConcurrentGateRecord::equals(const ConcurrentGateRecord *other) const noexcept {
	return this->gate == other->gate;
}

} /* namespace alinous */
