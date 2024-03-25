/*
 * DefaultInstanceMemberAllocator.cpp
 *
 *  Created on: 2023/05/18
 *      Author: iizuka
 */

#include "bc/DefaultInstanceMemberAllocator.h"

#include "bc_block_generator/BlockGenerator.h"

#include "bc_finalizer_pool/FinalizerPool.h"

namespace codablecash {

DefaultInstanceMemberAllocator::DefaultInstanceMemberAllocator() {

}

DefaultInstanceMemberAllocator::~DefaultInstanceMemberAllocator() {

}

BlockGenerator* DefaultInstanceMemberAllocator::newBlockGenerator(
		uint16_t zone, CodablecashConfig *config, MemoryPool *memoryPool,
		BlockchainController *ctrl, const MiningConfig *miningConfig, ISystemLogger* logger) {

	return new BlockGenerator(zone, config, memoryPool, ctrl, miningConfig, logger);
}

FinalizerPool* DefaultInstanceMemberAllocator::newFinalizerPool(
		const CodablecashConfig *config, MemoryPool *memoryPool, BlockchainController *ctrl
		, P2pRequestProcessor* p2pRequestProcessor, BlochchainP2pManager* p2pManager, ISystemLogger* logger) {
	return new FinalizerPool(config, memoryPool, ctrl, p2pRequestProcessor, p2pManager, logger);
}

} /* namespace codablecash */
