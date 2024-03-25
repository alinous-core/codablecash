/*
 * ZoneBlockchainSync.h
 *
 *  Created on: 2023/11/01
 *      Author: iizuka
 */

#ifndef BC_NETWORK_INSTANCE_SYNC_ZONEBLOCKCHAINSYNC_H_
#define BC_NETWORK_INSTANCE_SYNC_ZONEBLOCKCHAINSYNC_H_

#include "bc_network_instance_sync/AbstractZoneSync.h"

namespace codablecash {

class BlockHeaderTransferData;
class CodablecashNodeInstance;
class Block;

class ZoneBlockchainSync : public AbstractZoneSync {
public:
	ZoneBlockchainSync(uint16_t zone, NetworkNodeSync* parent, ISystemLogger* logger, const UnicodeString* name);
	virtual ~ZoneBlockchainSync();

protected:
	virtual void importResult();

private:
	void handleHeader(const BlockHeaderTransferData* data, CodablecashNodeInstance* inst);
	void addBlock(const Block* block, CodablecashNodeInstance* inst);
};

} /* namespace codablecash */

#endif /* BC_NETWORK_INSTANCE_SYNC_ZONEBLOCKCHAINSYNC_H_ */
