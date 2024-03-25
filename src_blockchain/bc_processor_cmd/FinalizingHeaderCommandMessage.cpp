/*
 * FinalizingHeaderCommandMessage.cpp
 *
 *  Created on: 2023/11/01
 *      Author: iizuka
 */

#include "bc_processor_cmd/FinalizingHeaderCommandMessage.h"

#include "bc_block/BlockHeaderId.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc_processor/CentralProcessor.h"

namespace codablecash {

FinalizingHeaderCommandMessage::FinalizingHeaderCommandMessage(uint16_t zone, uint64_t finalizingHeight, const BlockHeaderId *headerId) {
	this->zone = zone;
	this->finalizingHeight = finalizingHeight;
	this->headerId = dynamic_cast<BlockHeaderId*>(headerId->copyData());
}

FinalizingHeaderCommandMessage::~FinalizingHeaderCommandMessage() {
	delete this->headerId;
}

void FinalizingHeaderCommandMessage::process(CentralProcessor *processor) {
	MemoryPool* memPool = processor->getMemoryPool();
	BlockchainController* ctrl = processor->getCtrl();

	ctrl->finalizeHeader(this->zone, this->finalizingHeight, this->headerId, memPool);
}

} /* namespace codablecash */
