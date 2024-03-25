/*
 * HeaderSyncWorker.h
 *
 *  Created on: 2023/11/05
 *      Author: iizuka
 */

#ifndef BC_NETWORK_INSTANCE_SYNC_HEADERSYNCWORKER_H_
#define BC_NETWORK_INSTANCE_SYNC_HEADERSYNCWORKER_H_

#include "bc_network_instance_sync/AbstractSyncWorkerThread.h"

#include <cstdint>

using namespace alinous;

namespace codablecash {
class NodeIdentifier;
class AbstractZoneSync;
class ISystemLogger;
class BlochchainP2pManager;
class NodeIdentifierSource;

class HeaderSyncWorker : public AbstractSyncWorkerThread {
public:
	HeaderSyncWorker(uint16_t zone, const NodeIdentifier* nodeId, AbstractZoneSync* parent, ISystemLogger* logger, const UnicodeString* name);
	virtual ~HeaderSyncWorker();

protected:
	virtual void doProcess();

private:
	void syncHeader(BlochchainP2pManager *p2pManager, const NodeIdentifierSource *source, uint64_t finalizedHeight);

private:
	NodeIdentifier* nodeId;
	AbstractZoneSync* parent;
	uint16_t zone;

};

} /* namespace codablecash */

#endif /* BC_NETWORK_INSTANCE_SYNC_HEADERSYNCWORKER_H_ */
