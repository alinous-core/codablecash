/*
 * IBlockGenerationListner.h
 *
 *  Created on: Jun 16, 2026
 *      Author: iizuka
 */

#ifndef BC_BLOCK_GENERATOR_IBLOCKGENERATIONLISTNER_H_
#define BC_BLOCK_GENERATOR_IBLOCKGENERATIONLISTNER_H_

namespace codablecash {

class Block;
class MemPoolTransaction;
class BlockchainController;

class IBlockGenerationListner {
public:
	IBlockGenerationListner();
	virtual ~IBlockGenerationListner();

	virtual void onBlockGenerated(Block* block, MemPoolTransaction *memTrx, BlockchainController* ctrl) = 0;
	virtual IBlockGenerationListner* copy() const noexcept = 0;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_GENERATOR_IBLOCKGENERATIONLISTNER_H_ */
