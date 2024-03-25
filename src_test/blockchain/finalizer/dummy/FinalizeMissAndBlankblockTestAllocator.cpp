/*
 * FinalizeMissTestAllocator.cpp
 *
 *  Created on: 2023/05/24
 *      Author: iizuka
 */

#include "FinalizeMissAndBlankblockTestAllocator.h"

#include "FinalizeMissTestPool.h"

namespace codablecash {

FinalizeMissAndBlankblockTestAllocator::FinalizeMissAndBlankblockTestAllocator() {

}

FinalizeMissAndBlankblockTestAllocator::~FinalizeMissAndBlankblockTestAllocator() {

}

FinalizerPool* FinalizeMissAndBlankblockTestAllocator::newFinalizerPool(const codablecash::CodablecashConfig *config, MemoryPool *memoryPool,
		BlockchainController *ctrl, P2pRequestProcessor* p2pRequestProcessor, BlochchainP2pManager* p2pManager, ISystemLogger *logger) {
	return new FinalizeMissTestPool(config, memoryPool, ctrl, p2pRequestProcessor, p2pManager, logger);
}

IInstanceMemberAllocator* FinalizeMissAndBlankblockTestAllocator::copy() const noexcept {
	return new FinalizeMissAndBlankblockTestAllocator();
}

} /* namespace codablecash */
