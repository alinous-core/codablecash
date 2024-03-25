/*
 * ZoneHeaderSync.cpp
 *
 *  Created on: 2023/11/01
 *      Author: iizuka
 */

#include "bc_network_instance_sync/ZoneHeaderSync.h"

namespace codablecash {

ZoneHeaderSync::ZoneHeaderSync(uint16_t zone, NetworkNodeSync* parent, ISystemLogger* logger, const UnicodeString* name)
		: AbstractZoneSync(zone, parent, logger, name){

}

ZoneHeaderSync::~ZoneHeaderSync() {

}

void ZoneHeaderSync::importResult() {
}

} /* namespace codablecash */
