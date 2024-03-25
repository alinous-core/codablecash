/*
 * MerkleCertificateElement.cpp
 *
 *  Created on: 2022/03/22
 *      Author: iizuka
 */

#include "merkletree/MerkleCertificateElement.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"
namespace codablecash {

MerkleCertificateElement::MerkleCertificateElement(const MerkleCertificateElement &inst) {
	this->hash = inst.hash->clone();
	this->left = inst.left;
}

MerkleCertificateElement::MerkleCertificateElement(const ByteBuffer* hash, bool left) {
	this->hash = hash->clone();
	this->left = left;
}

MerkleCertificateElement::~MerkleCertificateElement() {
	delete this->hash;
}

int MerkleCertificateElement::binarySize() const {
	int total = sizeof(uint8_t) + this->hash->limit();
	total += sizeof(uint8_t);

	return total;
}

void MerkleCertificateElement::toBinary(ByteBuffer *out) const {
	uint8_t size = this->hash->limit();
	out->put(size);

	this->hash->position(0);
	const uint8_t* data = this->hash->array();
	out->put(data, size);

	uint8_t bl = this->left ? 1 : 0;
	out->put(bl);
}

MerkleCertificateElement* MerkleCertificateElement::createFromBinary(ByteBuffer *in) {
	uint8_t size = in->get();

	uint8_t* dest = new uint8_t[size];
	StackArrayRelease<uint8_t> __dest(dest);

	in->get(dest, size);
	ByteBuffer* buff = ByteBuffer::wrapWithEndian(dest, size, true); __STP(buff);

	uint8_t bl = in->get();
	bool left = (bl >= 1);

	return new MerkleCertificateElement(buff, left);
}

IBlockObject* MerkleCertificateElement::copyData() const noexcept {
	return new MerkleCertificateElement(*this);
}

} /* namespace codablecash */
