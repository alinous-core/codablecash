/*
 * MarkleTree.h
 *
 *  Created on: 2022/03/21
 *      Author: iizuka
 */

#ifndef MERKLETREE_MERKLETREE_H_
#define MERKLETREE_MERKLETREE_H_

#include "base/ArrayList.h"
#include <cstdint>

namespace alinous {
class ByteBuffer;
class IBlockObject;
}
using namespace alinous;

namespace codablecash {

class AbstractMerkleElement;
class MarkleElementFifo;
class MerkleCertificate;
class AbstractBlockchainTransaction;

class MerkleTree {
public:
	MerkleTree();
	virtual ~MerkleTree();

	void addElement(const IBlockObject* obj);
	void addElement(const AbstractBlockchainTransaction* trx);
	void addElement(uint64_t byte8) noexcept;
	void addElement(const ByteBuffer* b) noexcept;
	void addElement(const char* hash, int size) noexcept;

	void pack() noexcept;

	MerkleCertificate* makeCertificate(const ByteBuffer* b) const noexcept;
	MerkleCertificate* makeCertificate(const char* hash, int size) const noexcept;

	const AbstractMerkleElement* getRoot() const noexcept {
		return this->root;
	}

private:
	MarkleElementFifo* packFifo(MarkleElementFifo* fifo) const noexcept;


private:
	AbstractMerkleElement* root;

	ArrayList<ByteBuffer>* list;
};

} /* namespace codablecash */

#endif /* MERKLETREE_MERKLETREE_H_ */
