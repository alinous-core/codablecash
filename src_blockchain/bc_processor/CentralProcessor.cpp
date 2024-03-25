/*
 * CentralProcessor.cpp
 *
 *  Created on: 2023/04/01
 *      Author: iizuka
 */

#include "bc_processor/CentralProcessor.h"

#include "procesor/MessageProcessor.h"
#include "procesor/ICommandMessage.h"

#include "base/UnicodeString.h"

#include "bc_processor_cmd/MinedReportCommandMessage.h"
#include "bc_processor_cmd/FinalizingCommandMessage.h"
#include "bc_processor_cmd/FinalizingHeaderCommandMessage.h"

#include "base_thread/SysMutex.h"
#include "base_thread/StackUnlocker.h"

#include "bc_p2p_processor/P2pRequestProcessor.h"

namespace codablecash {

const UnicodeString CentralProcessor::THREAD_NAME(L"Central");

CentralProcessor::CentralProcessor(CodablecashConfig* config, MemoryPool* memPool, BlockchainController* ctrl, ISystemLogger* logger) {
	this->processor = nullptr;

	this->config = config;
	this->memPool = memPool;
	this->ctrl = ctrl;
	this->p2pRequestProcessor = nullptr;
	this->p2pManager = nullptr;
	this->logger = logger;

	this->p2pRequestProcessorMutex = new SysMutex();
	this->end = false;
}

CentralProcessor::~CentralProcessor() {
	shutdown();

	this->config = nullptr;
	this->memPool = nullptr;
	this->ctrl = nullptr;
	this->p2pRequestProcessor = nullptr;
	this->logger = nullptr;

	delete this->p2pRequestProcessorMutex;
}

void CentralProcessor::start() {
	StackUnlocker __lock(this->p2pRequestProcessorMutex);

	this->processor = new MessageProcessor(&THREAD_NAME, this->logger);
	this->processor->setParam(this);
	this->processor->start();
}

void CentralProcessor::shutdown() {
	{
		StackUnlocker __lock(this->p2pRequestProcessorMutex);
		this->end = true;
	}

	if(this->processor != nullptr){
		this->processor->shutdown();
		delete this->processor;
		this->processor = nullptr;
	}
}

void CentralProcessor::onMinedBlock(const Block *block) {
	StackUnlocker __lock(this->p2pRequestProcessorMutex);

	if(this->end == false){
		ICommandMessage* cmd = new MinedReportCommandMessage(block);

		this->processor->addCommandMessage(cmd);
	}
}

void CentralProcessor::requestFinalizing(uint16_t zone, uint64_t finalizingHeight, const BlockHeaderId *headerId) noexcept {
	StackUnlocker __lock(this->p2pRequestProcessorMutex);

	if(this->end == false){
		FinalizingCommandMessage* cmd = new FinalizingCommandMessage(zone, finalizingHeight, headerId);

		this->processor->addCommandMessage(cmd);
	}
}

void CentralProcessor::requestFinalizingHeader(uint16_t zone, uint64_t finalizingHeight, const BlockHeaderId *headerId) noexcept {
	StackUnlocker __lock(this->p2pRequestProcessorMutex);

	if(this->end == false){
		FinalizingHeaderCommandMessage* cmd = new FinalizingHeaderCommandMessage(zone, finalizingHeight, headerId);

		this->processor->addCommandMessage(cmd);
	}
}

void CentralProcessor::setBlochchainP2pManager(BlochchainP2pManager *p2pManager) noexcept {
	this->p2pManager = p2pManager;
}

void CentralProcessor::setP2pRequestProcessor(P2pRequestProcessor *p2pRequestProcessor) noexcept {
	StackUnlocker __lock(this->p2pRequestProcessorMutex);

	this->p2pRequestProcessor = p2pRequestProcessor;
}

void CentralProcessor::addCommandMessage(ICommandMessage *cmd) {
	StackUnlocker __lock(this->p2pRequestProcessorMutex);

	if(this->processor != nullptr && this->end == false){
		this->processor->addCommandMessage(cmd);
	}
}

} /* namespace codablecash */
