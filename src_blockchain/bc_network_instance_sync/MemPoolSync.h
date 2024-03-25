/*
 * MemPoolSync.h
 *
 *  Created on: 2023/11/03
 *      Author: iizuka
 */

#ifndef BC_NETWORK_INSTANCE_SYNC_MEMPOOLSYNC_H_
#define BC_NETWORK_INSTANCE_SYNC_MEMPOOLSYNC_H_

#include "bc_network_instance_sync/AbstractSyncWorkerThread.h"

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class NetworkNodeSync;
class MemPoolSyncWorker;

class MemPoolSync : public AbstractSyncWorkerThread {
public:
	MemPoolSync(NetworkNodeSync* parent, ISystemLogger* logger, const UnicodeString* name);
	virtual ~MemPoolSync();

protected:
	virtual void doProcess();

private:
	NetworkNodeSync* parent;
	ArrayList<MemPoolSyncWorker>* workers;
};

} /* namespace codablecash */

#endif /* BC_NETWORK_INSTANCE_SYNC_MEMPOOLSYNC_H_ */
