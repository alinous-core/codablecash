/*
 * NewShardAdder.cpp
 *
 *  Created on: Jun 24, 2026
 *      Author: iizuka
 */

#include "NewShardAdder.h"

#include "bc_block/Block.h"
#include "bc_block/BlockHeader.h"

#include "bc_block_header_command/NewShardZoneCommand.h"

namespace codablecash {

NewShardAdder::NewShardAdder(const NewShardAdder &inst) {
	this->height = inst.height;
}

NewShardAdder::NewShardAdder() {
	this->height = 0;
}

NewShardAdder::~NewShardAdder() {

}

void NewShardAdder::onBlockGenerated(Block *block, MemPoolTransaction *memTrx, BlockchainController *ctrl) {
	uint16_t height = block->getHeight();
	if(height == this->height){
		__addCommand(block, memTrx, ctrl);
	}
}

void NewShardAdder::__addCommand(Block *block, MemPoolTransaction *memTrx, BlockchainController *ctrl) {
	BlockHeader* header = block->getHeader();

	NewShardZoneCommand cmd;
	cmd.setNewShardNo(1);

	header->addHeaderCommand(&cmd);
}

IBlockGenerationListner* NewShardAdder::copy() const noexcept {
	return new NewShardAdder(*this);
}

void NewShardAdder::setHeight(uint64_t height) {
	this->height = height;
}

} /* namespace codablecash */
