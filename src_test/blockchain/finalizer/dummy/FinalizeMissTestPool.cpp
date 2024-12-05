/*
 * FinalizeMissTestPool.cpp
 *
 *  Created on: 2023/05/24
 *      Author: iizuka
 */

#include "FinalizeMissTestPool.h"

#include "FinalizeMissDetectTicketCommandMessage.h"

#include "procesor/MessageProcessor.h"

namespace codablecash {

FinalizeMissTestPool::FinalizeMissTestPool(const CodablecashSystemParam* config, MemoryPool* memoryPool, BlockchainController* ctrl
		, P2pRequestProcessor* p2pRequestProcessor, BlochchainP2pManager* p2pManager, ISystemLogger* logger)
		: FinalizerPool(config, memoryPool, ctrl, p2pRequestProcessor, p2pManager, logger){
	this->count = 0;
}

FinalizeMissTestPool::~FinalizeMissTestPool() {

}

void FinalizeMissTestPool::requestCheckVoting(uint16_t zone, const BlockHeaderId *headerId, uint64_t height) {
	int mod = this->count % 10;

	this->count++;

	if(this->count == 5){
		requestCheckVoting01(zone, headerId, height);
		return;
	}
	else if(mod == 2){
		requestCheckVoting02(zone, headerId, height);
		return;
	}

	FinalizerPool::requestCheckVoting(zone, headerId, height);
}

void FinalizeMissTestPool::requestCheckVoting01(uint16_t zone,	const BlockHeaderId *headerId, uint64_t height) {
	FinalizeMissDetectTicketCommandMessage* cmd = new FinalizeMissDetectTicketCommandMessage(zone, headerId, height);
	this->processor->addCommandMessage(cmd);
}

void FinalizeMissTestPool::requestCheckVoting02(uint16_t zone,	const BlockHeaderId *headerId, uint64_t height) {
	FinalizerPool::requestCheckVoting(zone, headerId, height);
}

} /* namespace codablecash */
