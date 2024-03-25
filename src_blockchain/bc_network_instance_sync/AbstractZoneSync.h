/*
 * AbstractZoneSync.h
 *
 *  Created on: 2023/11/01
 *      Author: iizuka
 */

#ifndef BC_NETWORK_INSTANCE_SYNC_ABSTRACTZONESYNC_H_
#define BC_NETWORK_INSTANCE_SYNC_ABSTRACTZONESYNC_H_

#include <cstdint>

#include "bc_network_instance_sync/AbstractSyncWorkerThread.h"

#include "base/ArrayList.h"

namespace alinous {
class SysMutex;
}
using namespace alinous;

namespace codablecash {

class NetworkNodeSync;
class HeaderSyncWorker;
class SyncHeaderRepo;
class CodablecashNetworkNode;
class SyncHeaderHeightData;

class AbstractZoneSync : public AbstractSyncWorkerThread {
public:
	AbstractZoneSync(uint16_t zone, NetworkNodeSync* parent, ISystemLogger* logger, const UnicodeString* name);
	virtual ~AbstractZoneSync();

	static AbstractZoneSync* createZone(uint16_t numZones, uint16_t zoneSelf, NetworkNodeSync* parent, ISystemLogger* logger);

	CodablecashNetworkNode* getCodablecashNetworkNode() const noexcept;

	void addHeaderData(const SyncHeaderHeightData* headerData);

protected:
	virtual void doProcess();

	void initHeadersRepo();
	void importHeaders() noexcept;

	virtual void importResult() = 0;

protected:
	NetworkNodeSync* parent;
	uint16_t zone;

	ArrayList<HeaderSyncWorker>* workers;

	SysMutex* headerDataMutex;
	SyncHeaderRepo* headerDataStore;
};

} /* namespace codablecash */

#endif /* BC_NETWORK_INSTANCE_SYNC_ABSTRACTZONESYNC_H_ */
