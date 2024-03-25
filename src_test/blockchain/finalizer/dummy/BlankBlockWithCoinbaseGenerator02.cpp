/*
 * BlankBlockWithCoinbaseGenerator02.cpp
 *
 *  Created on: 2023/12/03
 *      Author: iizuka
 */

#include "BlankBlockWithCoinbaseGenerator02.h"

#include "bc_status_cache/BlockchainController.h"

namespace codablecash {

BlankBlockWithCoinbaseGenerator02::BlankBlockWithCoinbaseGenerator02(int16_t zone, CodablecashConfig* config, MemoryPool* memoryPool
		, BlockchainController* ctrl, const MiningConfig *miningConfig, ISystemLogger* logger)
		: BlankBlockWithCoinbaseGenerator(zone, config, memoryPool, ctrl, miningConfig, logger){

}

BlankBlockWithCoinbaseGenerator02::~BlankBlockWithCoinbaseGenerator02() {

}

void BlankBlockWithCoinbaseGenerator02::nonceCalculated(uint64_t height,	const BlockHeaderId *bid, const PoWNonce *nonce) {
	uint64_t finalizedHeight = this->ctrl->getFinalizedHeight(zone);
	if(finalizedHeight >= height){
		return;
	}
	BlockGenerator::nonceCalculated(height, bid, nonce);

	doNonceCalculated(height, bid, nonce);
}

} /* namespace codablecash */
