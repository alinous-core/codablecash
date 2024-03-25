/*
 * TestBlockGenerator.h
 *
 *  Created on: 2023/05/22
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_POW_TESTBLOCKGENERATOR_H_
#define BLOCKCHAIN_POW_TESTBLOCKGENERATOR_H_

#include "bc_block_generator/BlockGenerator.h"

#include "numeric/BigInteger.h"

namespace codablecash {

class TestBlockGenerator : public BlockGenerator {
public:
	TestBlockGenerator();
	virtual ~TestBlockGenerator();

	virtual void nonceCalculated(uint64_t lastBlockHeight, const BlockHeaderId *bid, const PoWNonce *nonce);

	void waitUntilGetData();

	const PoWNonce* getNonce() const noexcept {
		return this->nonce;
	}

	void setTargetDiff(BigInteger& diff) noexcept;

	virtual BlockHeader* getTopBlockHeader() const;
	virtual Block* generateBlock(uint64_t lastBlockHeight, const BlockHeaderId *lastBlockId, MemPoolTransaction* memTrx);

	virtual void setScheduledBlock(const Block* block);

	virtual BigInteger calcTargetDiff(uint64_t lastHeight, const BlockHeaderId *lastHeaderId, const SystemTimestamp* tm);

	virtual bool continueMining() const;
	void setContinueMining(bool b) noexcept;
	void setMaxNonce();

	virtual BlockHeader* changeMiningTarget();

private:
	PoWNonce* nonce;
	Block* block;

	BigInteger targetDiff;
	bool bl;
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_POW_TESTBLOCKGENERATOR_H_ */
