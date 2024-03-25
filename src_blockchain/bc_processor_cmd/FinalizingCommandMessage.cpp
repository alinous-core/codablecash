/*
 * FinalizingCommandMessage.cpp
 *
 *  Created on: 2023/05/08
 *      Author: iizuka
 */

#include "bc_processor_cmd/FinalizingCommandMessage.h"

#include "bc_block/BlockHeaderId.h"

#include "bc_processor/CentralProcessor.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc_memorypool/MemPoolTransaction.h"
#include "bc_memorypool/MemoryPool.h"

#include "base/StackRelease.h"

namespace codablecash {

FinalizingCommandMessage::FinalizingCommandMessage(uint16_t zone, uint64_t finalizingHeight, const BlockHeaderId *headerId) {
	this->zone = zone;
	this->finalizingHeight = finalizingHeight;
	this->headerId = dynamic_cast<BlockHeaderId*>(headerId->copyData());
}

FinalizingCommandMessage::~FinalizingCommandMessage() {
	delete this->headerId;
}

void FinalizingCommandMessage::process(CentralProcessor *processor) {
	// clean memory pool. lock order memory -> cache
	MemoryPool* memPool = processor->getMemoryPool();
	BlockchainController* ctrl = processor->getCtrl();

	ctrl->finalize(this->zone, this->finalizingHeight, this->headerId, memPool);
}

} /* namespace codablecash */
