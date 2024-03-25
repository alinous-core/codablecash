/*
 * MemPoolSyncWorker.h
 *
 *  Created on: 2023/11/03
 *      Author: iizuka
 */

#ifndef BC_NETWORK_INSTANCE_SYNC_MEMPOOLSYNCWORKER_H_
#define BC_NETWORK_INSTANCE_SYNC_MEMPOOLSYNCWORKER_H_

#include "bc_network_instance_sync/AbstractSyncWorkerThread.h"

using namespace alinous;

namespace codablecash {

class NodeIdentifier;
class NetworkNodeSync;
class BlochchainP2pManager;
class MemoryPool;
class NodeIdentifierSource;

class MemPoolSyncWorker : public AbstractSyncWorkerThread {
public:
	MemPoolSyncWorker(const NodeIdentifier* nodeId, NetworkNodeSync* parent, ISystemLogger* logger, const UnicodeString* name);
	virtual ~MemPoolSyncWorker();

protected:
	virtual void doProcess();

private:
	void doSyncBalanceTransactions(BlochchainP2pManager* p2pManager, MemoryPool* pool, const NodeIdentifierSource *source, ISystemLogger* logger);
	void doSyncControlTransactions(BlochchainP2pManager* p2pManager, MemoryPool* pool, const NodeIdentifierSource *source, ISystemLogger* logger);
	void doSyncInterChainCommunicationTransactions(BlochchainP2pManager* p2pManager, MemoryPool* pool, const NodeIdentifierSource *source, ISystemLogger* logger);
	void doSyncSmartcontractTransactions(BlochchainP2pManager* p2pManager, MemoryPool* pool, const NodeIdentifierSource *source, ISystemLogger* logger);

private:
	NodeIdentifier* nodeId;
	NetworkNodeSync* parent;
};

} /* namespace codablecash */

#endif /* BC_NETWORK_INSTANCE_SYNC_MEMPOOLSYNCWORKER_H_ */
