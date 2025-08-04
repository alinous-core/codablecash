/*
 * BlockRewordBase.h
 *
 *  Created on: 2023/05/15
 *      Author: iizuka
 */

#ifndef BC_BLOCK_BODY_BLOCKREWORDBASE_H_
#define BC_BLOCK_BODY_BLOCKREWORDBASE_H_

#include "base/ArrayList.h"

#include <cstdint>

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class StakeBaseTransaction;
class CoinbaseTransaction;
class AbstractBlockRewordTransaction;
class MerkleTree;
class ITransactionVisitor;
class Block;

class BlockRewordBase {
public:
	BlockRewordBase(const BlockRewordBase& inst);
	BlockRewordBase();
	virtual ~BlockRewordBase();

	void addBlockRewordTransaction(const AbstractBlockRewordTransaction *trx) noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static BlockRewordBase* createFromBinary(ByteBuffer* in);

	void buildMerkleTree(MerkleTree* tree);

	const CoinbaseTransaction* getCoinbaseTransaction() const noexcept {
		return this->coinbase;
	}

	const ArrayList<StakeBaseTransaction>* getStakeBases() const noexcept {
		return this->stakeBases;
	}

	void visitTransactions(ITransactionVisitor* visitor, const Block* block) const;

private:
	CoinbaseTransaction* coinbase;
	ArrayList<StakeBaseTransaction>* stakeBases;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_BODY_BLOCKREWORDBASE_H_ */
