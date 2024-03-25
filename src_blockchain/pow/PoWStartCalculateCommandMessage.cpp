/*
 * PoWStartCalculateCommandMessage.cpp
 *
 *  Created on: 2023/03/29
 *      Author: iizuka
 */

#include "pow/PoWStartCalculateCommandMessage.h"
#include "pow/PoWManager.h"
#include "pow/AbstractPoWCalculator.h"

#include "bc_block/BlockHeaderId.h"
#include "bc_block/Block.h"
#include "bc_block/BlockHeader.h"

#include "numeric/BigInteger.h"

#include "procesor/ICommandParameter.h"

#include "bc_block_generator/BlockGenerator.h"

#include "bc_memorypool/MemoryPool.h"

#include "base/StackRelease.h"

#include "bc_memorypool/MemPoolTransaction.h"

#include "base_timestamp/SystemTimestamp.h"

#include "osenv/funcs.h"

#include "bc_block_body/BlockBody.h"
using alinous::BigInteger;
namespace codablecash {

PoWStartCalculateCommandMessage::PoWStartCalculateCommandMessage() {

}

PoWStartCalculateCommandMessage::~PoWStartCalculateCommandMessage() {

}

void PoWStartCalculateCommandMessage::process(PoWManager *manager) {
	BlockGenerator* generator = manager->getBlockGenerator();

	MemoryPool* memPool = generator->getMemoryPool();
	MemPoolTransaction* memTrx = memPool->begin(); __STP(memTrx); // mempol locking

	// check the head is not mining block
	BlockHeader* lastHeader = generator->getTopBlockHeader(); __STP(lastHeader);
	BlockHeader* targetLastBlock = nullptr;

	if(lastHeader->isScheduledBlock()){
		if(generator->continueMining()){ // if miner have to mine current block
			return;
		}

		// second to Top
		targetLastBlock = generator->changeMiningTarget();
	}
	else{
		targetLastBlock = dynamic_cast<BlockHeader*>(lastHeader->copyData());
	}
	__STP(targetLastBlock);

	// gnerate new Block to mine
	const BlockHeaderId* lastBlockHeaderId = targetLastBlock->getId();
	uint64_t lastBlockHeight = targetLastBlock->getHeight();
	Block* block = generator->generateBlock(lastBlockHeight, lastBlockHeaderId, memTrx); __STP(block);

	// update mining scheduled block
	generator->setScheduledBlock(block);

	{
		AbstractPoWCalculator* calc = manager->getCalculator();

		const SystemTimestamp* sysTm = block->getTimestamp();

		BigInteger targetDifficulty = generator->calcTargetDiff(lastBlockHeight, lastBlockHeaderId, sysTm);
		const BlockMerkleRoot* merkleRoot = block->getBody()->getMerkleRoot();

		calc->calculate(lastBlockHeight, lastBlockHeaderId, merkleRoot, sysTm, &targetDifficulty);
	}

}

} /* namespace codablecash */
