/*
 * MarkleElement.cpp
 *
 *  Created on: 2022/03/21
 *      Author: iizuka
 */

#include "merkletree/MerkleElement.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"

#include "crypto/Sha256.h"
#include "osenv/funcs.h"

#include "osenv/memory.h"
using alinous::Mem;
using alinous::Os;

namespace codablecash {

MerkleElement::MerkleElement() : AbstractMerkleElement() {

}

MerkleElement::~MerkleElement() {

}

void MerkleElement::calcHash() noexcept {
	int total = 0;

	int maxLoop = this->children->size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractMerkleElement* ele = this->children->get(i);

		total += ele->hashSize();
	}


	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true); __STP(buff);
	for(int i = 0; i != maxLoop; ++i){
		AbstractMerkleElement* ele = this->children->get(i);

		const ByteBuffer* b = ele->getHash();
		buff->put(b);
	}

	buff->position(0);
	this->hash = Sha256::sha256((const char*)buff->array(), buff->limit(), true);
}

const AbstractMerkleElement* MerkleElement::find(const ByteBuffer* hash) const noexcept {
	if(isLeaf()){
		int result = Mem::memcmp(this->hash->array(), hash->array(), hash->limit());

		return result == 0 ? this : nullptr;
	}

	const AbstractMerkleElement* ret = nullptr;

	int maxLoop = this->children->size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractMerkleElement* ele = this->children->get(i);

		ret = ele->find(hash);
		if(ret != nullptr){
			break;
		}
	}

	return ret;
}


} /* namespace codablecash */
