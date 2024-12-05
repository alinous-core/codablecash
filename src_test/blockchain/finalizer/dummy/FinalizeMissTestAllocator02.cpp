/*
 * FinalizeMissTestAllocator02.cpp
 *
 *  Created on: 2023/12/03
 *      Author: iizuka
 */

#include "FinalizeMissTestAllocator02.h"

#include "BlankBlockWithCoinbaseGenerator02.h"
#include "FinalizeMissTestPool.h"

namespace codablecash {

FinalizeMissTestAllocator02::FinalizeMissTestAllocator02() {

}

FinalizeMissTestAllocator02::~FinalizeMissTestAllocator02() {

}

BlockGenerator* FinalizeMissTestAllocator02::newBlockGenerator(uint16_t zone,
		CodablecashSystemParam *config, MemoryPool *memoryPool,
		BlockchainController *ctrl, const MiningConfig *miningConfig,
		ISystemLogger *logger) {
	return new BlankBlockWithCoinbaseGenerator02(zone, config, memoryPool, ctrl, miningConfig, logger);
}

FinalizerPool* FinalizeMissTestAllocator02::newFinalizerPool(
		const codablecash::CodablecashSystemParam *config, MemoryPool *memoryPool,
		BlockchainController *ctrl, P2pRequestProcessor *p2pRequestProcessor,
		BlochchainP2pManager *p2pManager, ISystemLogger *logger) {
	return new FinalizeMissTestPool(config, memoryPool, ctrl, p2pRequestProcessor, p2pManager, logger);
}

IInstanceMemberAllocator* FinalizeMissTestAllocator02::copy() const noexcept {
	return new FinalizeMissTestAllocator02();
}

} /* namespace codablecash */
