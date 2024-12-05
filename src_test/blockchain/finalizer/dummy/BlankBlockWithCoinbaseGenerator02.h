/*
 * BlankBlockWithCoinbaseGenerator02.h
 *
 *  Created on: 2023/12/03
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_FINALIZER_DUMMY_BLANKBLOCKWITHCOINBASEGENERATOR02_H_
#define BLOCKCHAIN_FINALIZER_DUMMY_BLANKBLOCKWITHCOINBASEGENERATOR02_H_

#include "BlankBlockWithCoinbaseGenerator.h"

namespace codablecash {

class BlankBlockWithCoinbaseGenerator02 : public BlankBlockWithCoinbaseGenerator {
public:
	BlankBlockWithCoinbaseGenerator02(int16_t zone, CodablecashSystemParam* config, MemoryPool* memoryPool, BlockchainController* ctrl, const MiningConfig *miningConfig, ISystemLogger* logger);
	virtual ~BlankBlockWithCoinbaseGenerator02();

	virtual void nonceCalculated(uint64_t height, const BlockHeaderId *bid, const PoWNonce *nonce);
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_FINALIZER_DUMMY_BLANKBLOCKWITHCOINBASEGENERATOR02_H_ */
