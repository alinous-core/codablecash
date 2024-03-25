/*
 * BlankBlockGeneratorAllocator.cpp
 *
 *  Created on: 2023/05/19
 *      Author: iizuka
 */

#include "BlankBlockGeneratorAllocator.h"

#include "BlankBlockGenerator.h"
namespace codablecash {

BlankBlockGeneratorAllocator::BlankBlockGeneratorAllocator() {

}

BlankBlockGeneratorAllocator::~BlankBlockGeneratorAllocator() {

}

BlockGenerator* BlankBlockGeneratorAllocator::newBlockGenerator(uint16_t zone,
		CodablecashConfig *config, MemoryPool *memoryPool,
		BlockchainController *ctrl, const MiningConfig *miningConfig, ISystemLogger* logger) {
	return new BlankBlockGenerator(zone, config, memoryPool, ctrl, miningConfig, logger);
}

} /* namespace codablecash */
