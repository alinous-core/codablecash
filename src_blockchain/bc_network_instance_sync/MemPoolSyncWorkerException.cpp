/*
 * MemPoolSyncWorkerException.cpp
 *
 *  Created on: 2023/11/05
 *      Author: iizuka
 */

#include "bc_network_instance_sync/MemPoolSyncWorkerException.h"

#include "base/UnicodeString.h"

namespace codablecash {

const wchar_t* MemPoolSyncWorkerException::defaultMessage = L"Memory Pool synchronization error. ";

MemPoolSyncWorkerException::MemPoolSyncWorkerException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
MemPoolSyncWorkerException::MemPoolSyncWorkerException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
MemPoolSyncWorkerException::MemPoolSyncWorkerException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
MemPoolSyncWorkerException::MemPoolSyncWorkerException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
MemPoolSyncWorkerException::~MemPoolSyncWorkerException() {
}

} /* namespace codablecash */
