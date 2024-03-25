/*
 * ZoneBlockValidator.cpp
 *
 *  Created on: 2023/11/19
 *      Author: iizuka
 */

#include "bc_network_instance_sync/ZoneBlockValidator.h"

#include "bc_p2p_cmd_node/DownloadBlockNodeCommandResponse.h"

#include "bc_block/Block.h"
#include "bc_block/BlockHeader.h"
#include "bc_block/BlockHeaderId.h"


namespace codablecash {

ZoneBlockValidator::ZoneBlockValidator(uint16_t zone, uint64_t height, const BlockHeaderId* headerId) {
	this->zone = zone;
	this->height = height;
	this->headerId = headerId;
}

ZoneBlockValidator::~ZoneBlockValidator() {

}

bool ZoneBlockValidator::validate(AbstractCommandResponse *response) {
	bool ret = false;

	DownloadBlockNodeCommandResponse* dlresponse = dynamic_cast<DownloadBlockNodeCommandResponse*>(response);
	if(dlresponse != nullptr){
		Block* block = dlresponse->getBlock();
		BlockHeader* header = block->getHeader();

		uint16_t z = header->getZone();
		uint64_t h = header->getHeight();
		BlockHeaderId* id = header->getId();

		ret = (this->zone == z && this->height == h && this->headerId->equals(id));
	}

	return ret;
}

} /* namespace codablecash */
