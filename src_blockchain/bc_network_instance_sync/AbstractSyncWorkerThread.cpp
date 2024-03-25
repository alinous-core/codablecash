/*
 * AbstractSyncWorkerThread.cpp
 *
 *  Created on: 2023/11/03
 *      Author: iizuka
 */

#include "bc_network_instance_sync/AbstractSyncWorkerThread.h"

#include "base/StackRelease.h"
#include "base/Exception.h"

#include "bc/ISystemLogger.h"

namespace codablecash {

AbstractSyncWorkerThread::AbstractSyncWorkerThread(ISystemLogger* logger, const UnicodeString* name) : AbstractThreadRunner(name) {
	this->logger = logger;
}

AbstractSyncWorkerThread::~AbstractSyncWorkerThread() {
	this->logger = nullptr;
}

void AbstractSyncWorkerThread::process() noexcept {
	try{
		doProcess();
	}
	catch(Exception* e){
		__STP(e);
		this->logger->logException(e);
	}
}

} /* namespace codablecash */
