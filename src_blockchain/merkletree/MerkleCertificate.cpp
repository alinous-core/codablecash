/*
 * MerkleCertificate.cpp
 *
 *  Created on: 2022/03/22
 *      Author: iizuka
 */

#include "merkletree/MerkleCertificate.h"
#include "merkletree/MerkleCertificateElement.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"

#include "../../src/crypto/Sha256.h"
#include "osenv/memory.h"

#include "bc_base/BinaryUtils.h"
using alinous::Mem;
namespace codablecash {

MerkleCertificate::MerkleCertificate(const MerkleCertificate &inst) {
	this->rootHash = inst.rootHash != nullptr ? inst.rootHash->clone() : nullptr;

	int maxLoop = inst.list.size();
	for(int i = 0; i != maxLoop; ++i){
		MerkleCertificateElement* el = inst.list.get(i);

		this->list.addElement(dynamic_cast<MerkleCertificateElement*>(el->copyData()));
	}
}

MerkleCertificate::MerkleCertificate() {
	this->rootHash = nullptr;
}

MerkleCertificate::~MerkleCertificate() {
	delete this->rootHash;

	this->list.deleteElements();
}

void MerkleCertificate::setMerkleRoot(const ByteBuffer* hash) noexcept {
	this->rootHash = hash->clone();
}

void MerkleCertificate::addHash(MerkleCertificateElement* element) noexcept {
	this->list.addElement(element);
}

bool MerkleCertificate::certificate() const {
	ArrayList<MerkleCertificateElement> fifo;
	fifo.addAll(&this->list);

	MerkleCertificateElement* first = fifo.remove(0);
	ByteBuffer* hash = first->getHash()->clone();

	while(!fifo.isEmpty()){
		MerkleCertificateElement* element = fifo.remove(0);
		const ByteBuffer* elHash = element->getHash();

		ByteBuffer* h = nullptr;
		if(element->isLeft()){
			h = joinHash(elHash, hash);
		}else{
			h = joinHash(hash, elHash);
		}

		delete hash, hash = h;
	}

	int result = Mem::memcmp(this->rootHash->array(), hash->array(), this->rootHash->limit());

	delete hash;

	return result == 0;
}

ByteBuffer* MerkleCertificate::joinHash(const ByteBuffer* left,	const ByteBuffer* right) const noexcept {
	int total = left->limit() + right->limit();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true); __STP(buff);

	buff->put(left);
	buff->put(right);

	buff->position(0);

	return Sha256::sha256((const char*)buff->array(), total, true);
}

int MerkleCertificate::binarySize() const {
	BinaryUtils::checkNotNull(this->rootHash);

	int total = sizeof(uint8_t);
	total += this->rootHash->limit();

	int maxLoop = this->list.size();
	total += sizeof(uint16_t);

	for(int i = 0; i != maxLoop; ++i){
		MerkleCertificateElement* cert = this->list.get(i);

		total += cert->binarySize();
	}

	return total;
}

void MerkleCertificate::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->rootHash);

	uint8_t size = this->rootHash->limit();
	out->put(size);

	out->put(this->rootHash->array(), size);

	int maxLoop = this->list.size();
	out->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		MerkleCertificateElement* cert = this->list.get(i);

		cert->toBinary(out);
	}
}

MerkleCertificate* MerkleCertificate::createFromBinary(ByteBuffer *in) {
	MerkleCertificate* inst = new MerkleCertificate(); __STP(inst);

	uint8_t size = in->get();
	uint8_t* data = new uint8_t[size];
	StackArrayRelease<uint8_t> __data(data);

	in->get(data, size);
	inst->rootHash = ByteBuffer::wrapWithEndian(data, size, true);

	int maxLoop = in->getShort();
	for(int i = 0; i != maxLoop; ++i){
		MerkleCertificateElement* cert = MerkleCertificateElement::createFromBinary(in);

		inst->list.addElement(cert);
	}

	return __STP_MV(inst);
}

IBlockObject* MerkleCertificate::copyData() const noexcept {
	return new MerkleCertificate(*this);
}

ByteBuffer* MerkleCertificate::getRootHash() const noexcept {
	return this->rootHash->clone();
}

} /* namespace codablecash */
