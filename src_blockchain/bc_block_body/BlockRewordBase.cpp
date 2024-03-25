/*
 * BlockRewordBase.cpp
 *
 *  Created on: 2023/05/15
 *      Author: iizuka
 */

#include "bc_block_body/BlockRewordBase.h"
#include "bc_block_body/StakeBaseTransaction.h"
#include "bc_block_body/CoinbaseTransaction.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

#include "merkletree/MerkleTree.h"

namespace codablecash {

BlockRewordBase::BlockRewordBase(const BlockRewordBase &inst) {
	this->coinbase = inst.coinbase != nullptr ? dynamic_cast<CoinbaseTransaction*>(inst.coinbase->copyData()) : nullptr;

	this->stakeBases = new ArrayList<StakeBaseTransaction>();

	int maxLoop = inst.stakeBases->size();
	for(int i = 0; i != maxLoop; ++i){
		const StakeBaseTransaction* trx = inst.stakeBases->get(i);

		addBlockRewordTransaction(trx);
	}
}

BlockRewordBase::BlockRewordBase() {
	this->coinbase = nullptr;
	this->stakeBases = new ArrayList<StakeBaseTransaction>();
}

BlockRewordBase::~BlockRewordBase() {
	delete this->coinbase;

	this->stakeBases->deleteElements();
	delete this->stakeBases;
}

void BlockRewordBase::addBlockRewordTransaction(const AbstractBlockRewordTransaction *trx) noexcept {
	uint8_t type = trx->getType();
	if(type == AbstractBlockchainTransaction::TRX_TYPE_COIN_BASE){
		this->coinbase = dynamic_cast<CoinbaseTransaction*>(trx->copyData());
		assert(this->coinbase != nullptr);
		return;
	}

	StakeBaseTransaction* strx = dynamic_cast<StakeBaseTransaction*>(trx->copyData());
	assert(strx != nullptr);
	this->stakeBases->addElement(strx);
}

int BlockRewordBase::binarySize() const {
	int total = sizeof(uint8_t);

	if(this->coinbase != nullptr){
		total += this->coinbase->binarySize();
	}

	int maxLoop = this->stakeBases->size();
	total += sizeof(uint8_t);

	for(int i = 0; i != maxLoop; ++i){
		const StakeBaseTransaction* trx = this->stakeBases->get(i);

		total += trx->binarySize();
	}

	return total;
}

void BlockRewordBase::toBinary(ByteBuffer *out) const {
	bool bl = this->coinbase != nullptr;
	out->put(bl ? 1 : 0);
	if(bl){
		this->coinbase->toBinary(out);
	}

	int maxLoop = this->stakeBases->size();
	out->put(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		const StakeBaseTransaction* trx = this->stakeBases->get(i);

		trx->toBinary(out);
	}
}

BlockRewordBase* BlockRewordBase::createFromBinary(ByteBuffer *in) {
	BlockRewordBase* base = new BlockRewordBase(); __STP(base);

	uint8_t bl = in->get();
	if(bl > 0){
		AbstractBlockchainTransaction* trx = AbstractBlockchainTransaction::createFromBinary(in); __STP(trx);
		base->coinbase = dynamic_cast<CoinbaseTransaction*>(trx);
		BinaryUtils::checkNotNull(base->coinbase);
		__STP_MV(trx);
	}

	int maxLoop = in->get();
	for(int i = 0; i != maxLoop; ++i){
		AbstractBlockchainTransaction* trx = AbstractBlockchainTransaction::createFromBinary(in); __STP(trx);
		StakeBaseTransaction* stakeTrx = dynamic_cast<StakeBaseTransaction*>(trx);
		BinaryUtils::checkNotNull(stakeTrx);

		base->addBlockRewordTransaction(stakeTrx);
	}

	return __STP_MV(base);
}

void BlockRewordBase::buildMerkleTree(MerkleTree *tree) {
	if(this->coinbase != nullptr){
		tree->addElement(this->coinbase);
	}

	int maxLoop = this->stakeBases->size();
	for(int i = 0; i != maxLoop; ++i){
		const StakeBaseTransaction* trx = this->stakeBases->get(i);

		tree->addElement(trx);
	}
}

} /* namespace codablecash */
