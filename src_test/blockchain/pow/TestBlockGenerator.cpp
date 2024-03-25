/*
 * TestBlockGenerator.cpp
 *
 *  Created on: 2023/05/22
 *      Author: iizuka
 */

#include "TestBlockGenerator.h"

#include "bc_block/Block.h"
#include "bc_block/BlockHeader.h"

#include "pow/PoWNonce.h"

#include "osenv/funcs.h"


using alinous::Os;

namespace codablecash {

TestBlockGenerator::TestBlockGenerator() : BlockGenerator(0, nullptr, nullptr, nullptr, nullptr, nullptr), targetDiff((int64_t)0L) {
	this->nonce = nullptr;

	this->block = new Block(0, 1);
	this->block->build();

	this->bl = false;
}

TestBlockGenerator::~TestBlockGenerator() {
	delete this->nonce;
	delete this->block;
}

void TestBlockGenerator::nonceCalculated(uint64_t lastBlockHeight,
		const BlockHeaderId *bid, const PoWNonce *nonce) {
	delete this->nonce;
	this->nonce = dynamic_cast<PoWNonce*>(nonce->copyData());

}

void TestBlockGenerator::waitUntilGetData() {
	while(this->nonce == nullptr){
		Os::usleep(10000);
	}
}

BlockHeader* TestBlockGenerator::getTopBlockHeader() const {
	const BlockHeader* h = this->block->getHeader();

	return dynamic_cast<BlockHeader*>(h->copyData());
}

BlockHeader* TestBlockGenerator::changeMiningTarget() {
	return getTopBlockHeader();
}

Block* TestBlockGenerator::generateBlock(uint64_t lastBlockHeight, const BlockHeaderId *lastBlockId, MemPoolTransaction *memTrx) {
	return new Block(*this->block);
}

void TestBlockGenerator::setScheduledBlock(const Block *block) {
}

BigInteger TestBlockGenerator::calcTargetDiff(uint64_t lastHeight, const BlockHeaderId *lastHeaderId, const SystemTimestamp *tm) {
	return this->targetDiff;
}

void TestBlockGenerator::setTargetDiff(BigInteger &diff) noexcept {
	this->targetDiff = diff;
}

bool TestBlockGenerator::continueMining() const {
	return this->bl;
}

void TestBlockGenerator::setContinueMining(bool b) noexcept {
	this->bl = b;
}

void TestBlockGenerator::setMaxNonce() {
	BlockHeader* header = this->block->getHeader();
	header->setPoWNonce(&PoWNonce::MAX_NONCE);
}

} /* namespace codablecash */
