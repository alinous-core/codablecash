/*
 * ZoneHeaderSync.h
 *
 *  Created on: 2023/11/01
 *      Author: iizuka
 */

#ifndef BC_NETWORK_INSTANCE_SYNC_ZONEHEADERSYNC_H_
#define BC_NETWORK_INSTANCE_SYNC_ZONEHEADERSYNC_H_

#include "bc_network_instance_sync/AbstractZoneSync.h"

namespace codablecash {

class ZoneHeaderSync : public AbstractZoneSync {
public:
	ZoneHeaderSync(uint16_t zone, NetworkNodeSync* parent, ISystemLogger* logger, const UnicodeString* name);
	virtual ~ZoneHeaderSync();

protected:
	virtual void importResult();
};

} /* namespace codablecash */

#endif /* BC_NETWORK_INSTANCE_SYNC_ZONEHEADERSYNC_H_ */
