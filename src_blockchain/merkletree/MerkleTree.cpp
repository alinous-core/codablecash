/*
 * MarkleTree.cpp
 *
 *  Created on: 2022/03/21
 *      Author: iizuka
 */

#include "merkletree/MerkleTree.h"
#include "merkletree/AbstractMerkleElement.h"
#include "merkletree/MarkleElementFifo.h"
#include "merkletree/MerkleElement.h"
#include "merkletree/MerkleCertificate.h"
#include "merkletree/BlankMerkleElement.h"
#include "merkletree/MerkleCertificateElement.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"

#include "crypto/Sha256.h"

#include "filestore_block/IBlockObject.h"

#include "bc_trx/AbstractBlockchainTransaction.h"

#include "bc_trx/TransactionId.h"
namespace codablecash {

MerkleTree::MerkleTree() {
	this->root = nullptr;
	this->list = new ArrayList<ByteBuffer>();
}

MerkleTree::~MerkleTree() {
	delete this->root;

	this->list->deleteElements();
	delete this->list;
}

void MerkleTree::addElement(const IBlockObject* obj) {
	int size = obj->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	obj->toBinary(buff);

	buff->position(0);

	ByteBuffer* hash = Sha256::sha256(buff, true); __STP(hash);
	hash->position(0);

	addElement((const char*)hash->array(), hash->limit());
}

void MerkleTree::addElement(const AbstractBlockchainTransaction *trx) {
	const TransactionId* trxId = trx->getTransactionId();

	int size = trxId->size();
	const char* hash = trxId->toArray();

	addElement(hash, size);
}

void MerkleTree::addElement(uint64_t byte8) noexcept {
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(sizeof(uint64_t), true); __STP(buff);
	buff->putLong(byte8);

	buff->position(0);
	ByteBuffer* hash = Sha256::sha256(buff, true); __STP(hash);

	hash->position(0);
	addElement(hash);
}

void MerkleTree::addElement(const ByteBuffer* b) noexcept {
	addElement((const char*)b->array(), b->limit());
}

void MerkleTree::addElement(const char* hash, int size) noexcept {
	ByteBuffer* buff = ByteBuffer::wrapWithEndian((const uint8_t*)hash, size, true);
	this->list->addElement(buff);
}

void MerkleTree::pack() noexcept {
	MarkleElementFifo* fifo = new MarkleElementFifo();

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		ByteBuffer* buff = this->list->get(i);

		MerkleElement* element = new MerkleElement();
		element->setHash(buff);

		fifo->addElement(element);
	}

	while(fifo->size() != 1){
		MarkleElementFifo* lastFifo = fifo; __STP(lastFifo);

		fifo = packFifo(lastFifo);
	}

	this->root = fifo->out();

	delete fifo;
}

MarkleElementFifo* MerkleTree::packFifo(MarkleElementFifo* fifo) const noexcept {
	MarkleElementFifo* newFifo = new MarkleElementFifo();

	while(!fifo->isEmpty()){
		MerkleElement* newElement = new MerkleElement();
		newFifo->addElement(newElement);

		AbstractMerkleElement* ele = fifo->out();
		newElement->addChild(ele);

		if(!fifo->isEmpty()){
			ele = fifo->out();
			newElement->addChild(ele);
		}else{
			// blank node
			newElement->addChild(new BlankMerkleElement());
		}

		newElement->calcHash();
	}

	return newFifo;
}

MerkleCertificate* MerkleTree::makeCertificate(const ByteBuffer* b) const noexcept {
	MerkleCertificate* cert = new MerkleCertificate(); __STP(cert);

	const ByteBuffer* rootHash = this->root->getHash();
	cert->setMerkleRoot(rootHash);

	const AbstractMerkleElement* element = this->root->find(b);
	if(element == nullptr){
		return nullptr;
	}
	else if(element->isRoot()){
		const ByteBuffer* hash = element->getHash();
		MerkleCertificateElement* me = new MerkleCertificateElement(hash, true);
		cert->addHash(me);

		return __STP_MV(cert);
	}
	else {
		const ByteBuffer* hash = element->getHash();
		MerkleCertificateElement* me = new MerkleCertificateElement(hash, element->isLeft());
		cert->addHash(me);
	}

	while(!element->isRoot()){
		element = element->getAnotherPair();
		const ByteBuffer* hash = element->getHash();

		MerkleCertificateElement* me = new MerkleCertificateElement(hash, element->isLeft());
		cert->addHash(me);

		element = element->getParent();
	}

	return __STP_MV(cert);
}

MerkleCertificate* MerkleTree::makeCertificate(const char* hash, int size) const noexcept {
	ByteBuffer* buff = ByteBuffer::wrapWithEndian((const uint8_t*)hash, size, true); __STP(buff);

	return makeCertificate(buff);
}

} /* namespace codablecash */
