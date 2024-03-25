/*
 * FinalizerPool.cpp
 *
 *  Created on: 2023/05/02
 *      Author: iizuka
 */

#include "bc_finalizer_pool/FinalizerPool.h"
#include "procesor/MessageProcessor.h"

#include "base/UnicodeString.h"

#include "bc/CodablecashConfig.h"
#include "bc_finalizer_pool/DetectVotingTicketCommandMessage.h"

#include "bc_memorypool/MemoryPool.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_network/NodeIdentifierSource.h"

namespace codablecash {

const UnicodeString FinalizerPool::THREAD_NAME{L"Finalizer"};

FinalizerPool::FinalizerPool(const CodablecashConfig* config, MemoryPool* memoryPool, BlockchainController* ctrl
		, P2pRequestProcessor* p2pRequestProcessor, BlochchainP2pManager* p2pManager, ISystemLogger* logger) {
	this->processor = nullptr;

	this->config = config;
	this->memoryPool = memoryPool;
	this->ctrl = ctrl;
	this->p2pRequestProcessor = p2pRequestProcessor;
	this->p2pManager = p2pManager;

	this->voterSource = nullptr;
	this->logger = logger;
}

FinalizerPool::~FinalizerPool() {
	shutdown();

	this->voterSource = nullptr;
}

void FinalizerPool::start() {
	this->processor = new MessageProcessor(&THREAD_NAME, this->logger);
	this->processor->setParam(this);
	this->processor->start();
}

void FinalizerPool::shutdown() noexcept {
	if(this->processor != nullptr){
		this->processor->shutdown();
	}

	delete this->processor;
	this->processor = nullptr;

}

void FinalizerPool::requestCheckVoting(uint16_t zone, const BlockHeaderId *headerId, uint64_t height) {
	DetectVotingTicketCommandMessage* cmd = new DetectVotingTicketCommandMessage(zone, headerId, height);
	this->processor->addCommandMessage(cmd);
}


const NodeIdentifier FinalizerPool::getNodeIdentifier() const noexcept {
	return this->voterSource->toNodeIdentifier();
}

void FinalizerPool::setVoterIdentifier(const NodeIdentifierSource *nodeSource) {
	this->voterSource = nodeSource;
}

} /* namespace codablecash */
