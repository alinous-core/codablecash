/*
 * BlankWithCoinbaseGenerator.h
 *
 *  Created on: 2023/12/03
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_FINALIZER_DUMMY_BLANKBLOCKWITHCOINBASEGENERATOR_H_
#define BLOCKCHAIN_FINALIZER_DUMMY_BLANKBLOCKWITHCOINBASEGENERATOR_H_

#include "bc_block_generator/BlockGenerator.h"

namespace codablecash {

class BlankBlockWithCoinbaseGenerator : public BlockGenerator {
public:
	BlankBlockWithCoinbaseGenerator(int16_t zone, CodablecashConfig* config, MemoryPool* memoryPool, BlockchainController* ctrl, const MiningConfig *miningConfig, ISystemLogger* logger);
	virtual ~BlankBlockWithCoinbaseGenerator();

	virtual void nonceCalculated(uint64_t height, const BlockHeaderId *bid, const PoWNonce *nonce);

protected:
	void doNonceCalculated(uint64_t lastBlockHeight, const BlockHeaderId *bid, const PoWNonce *nonce);
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_FINALIZER_DUMMY_BLANKBLOCKWITHCOINBASEGENERATOR_H_ */
