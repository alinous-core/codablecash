/*
 * MemPoolSyncWorkerException.h
 *
 *  Created on: 2023/11/05
 *      Author: iizuka
 */

#ifndef BC_NETWORK_INSTANCE_SYNC_MEMPOOLSYNCWORKEREXCEPTION_H_
#define BC_NETWORK_INSTANCE_SYNC_MEMPOOLSYNCWORKEREXCEPTION_H_

#include "base/Exception.h"

using namespace alinous;

namespace codablecash {

class MemPoolSyncWorkerException : public Exception {
public:
	MemPoolSyncWorkerException(const char* srcfile, int srcline) noexcept;
	MemPoolSyncWorkerException(Exception* cause, const char* srcfile, int srcline) noexcept;
	MemPoolSyncWorkerException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	MemPoolSyncWorkerException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~MemPoolSyncWorkerException();
	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* BC_NETWORK_INSTANCE_SYNC_MEMPOOLSYNCWORKEREXCEPTION_H_ */
