/*
 * ZoneBlockValidator.h
 *
 *  Created on: 2023/11/19
 *      Author: iizuka
 */

#ifndef BC_NETWORK_INSTANCE_SYNC_ZONEBLOCKVALIDATOR_H_
#define BC_NETWORK_INSTANCE_SYNC_ZONEBLOCKVALIDATOR_H_

#include "bc_p2p_cmd_node_access/IRetryableResponceValidator.h"

#include <cstdint>


namespace codablecash {

class BlockHeaderId;

class ZoneBlockValidator : public IRetryableResponceValidator {
public:
	ZoneBlockValidator(uint16_t zone, uint64_t height, const BlockHeaderId* headerId);
	virtual ~ZoneBlockValidator();

	virtual bool validate(AbstractCommandResponse* response);

private:
	uint16_t zone;
	uint64_t height;
	const BlockHeaderId* headerId;
};

} /* namespace codablecash */

#endif /* BC_NETWORK_INSTANCE_SYNC_ZONEBLOCKVALIDATOR_H_ */
