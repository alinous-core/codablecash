/*
 * FinalizeMissTestAllocator.cpp
 *
 *  Created on: 2023/12/03
 *      Author: iizuka
 */

#include "FinalizeMissTestAllocator.h"

#include "BlankBlockWithCoinbaseGenerator.h"
#include "FinalizeMissTestPool.h"

namespace codablecash {

FinalizeMissTestAllocator::FinalizeMissTestAllocator() {

}

FinalizeMissTestAllocator::~FinalizeMissTestAllocator() {

}

BlockGenerator* FinalizeMissTestAllocator::newBlockGenerator(uint16_t zone,
		CodablecashSystemParam *config, MemoryPool *memoryPool,
		BlockchainController *ctrl, const MiningConfig *miningConfig,
		ISystemLogger *logger) {
	return new BlankBlockWithCoinbaseGenerator(zone, config, memoryPool, ctrl, miningConfig, logger);
}

FinalizerPool* FinalizeMissTestAllocator::newFinalizerPool(
		const codablecash::CodablecashSystemParam *config, MemoryPool *memoryPool,
		BlockchainController *ctrl, P2pRequestProcessor *p2pRequestProcessor,
		BlochchainP2pManager *p2pManager, ISystemLogger *logger) {
	return new FinalizeMissTestPool(config, memoryPool, ctrl, p2pRequestProcessor, p2pManager, logger);
}

IInstanceMemberAllocator* FinalizeMissTestAllocator::copy() const noexcept {
	return new FinalizeMissTestAllocator();
}

} /* namespace codablecash */
