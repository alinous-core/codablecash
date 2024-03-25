/*
 * BlankBlockGenerator.h
 *
 *  Created on: 2023/05/19
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_FINALIZER_DUMMY_BLANKBLOCKGENERATOR_H_
#define BLOCKCHAIN_FINALIZER_DUMMY_BLANKBLOCKGENERATOR_H_

#include "bc_block_generator/BlockGenerator.h"

namespace codablecash {

class BlankBlockGenerator : public BlockGenerator {
public:
	BlankBlockGenerator(int16_t zone, CodablecashConfig* config, MemoryPool* memoryPool, BlockchainController* ctrl, const MiningConfig *miningConfig, ISystemLogger* logger);
	virtual ~BlankBlockGenerator();

	virtual void nonceCalculated(uint64_t height, const codablecash::BlockHeaderId *bid, const codablecash::PoWNonce *nonce);

private:
	void doNonceCalculated(uint64_t height, const BlockHeaderId *bid, const PoWNonce *nonce);
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_FINALIZER_DUMMY_BLANKBLOCKGENERATOR_H_ */
