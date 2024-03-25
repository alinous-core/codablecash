/*
 * AbstractSyncWorkerThread.h
 *
 *  Created on: 2023/11/03
 *      Author: iizuka
 */

#ifndef BC_NETWORK_INSTANCE_SYNC_ABSTRACTSYNCWORKERTHREAD_H_
#define BC_NETWORK_INSTANCE_SYNC_ABSTRACTSYNCWORKERTHREAD_H_

#include "base_thread/AbstractThreadRunner.h"

using namespace alinous;

namespace codablecash {

class ISystemLogger;

class AbstractSyncWorkerThread : public AbstractThreadRunner {
public:
	AbstractSyncWorkerThread(ISystemLogger* logger, const UnicodeString* name);
	virtual ~AbstractSyncWorkerThread();

protected:
	virtual void process() noexcept;
	virtual void doProcess() = 0;

protected:
	ISystemLogger* logger;
};

} /* namespace codablecash */

#endif /* BC_NETWORK_INSTANCE_SYNC_ABSTRACTSYNCWORKERTHREAD_H_ */
