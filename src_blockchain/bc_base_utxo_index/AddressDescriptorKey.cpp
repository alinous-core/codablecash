/*
 * AddressDescriptorKey.cpp
 *
 *  Created on: 2023/02/23
 *      Author: iizuka
 */

#include "bc_base_utxo_index/AddressDescriptorKey.h"
#include "bc_base_utxo_index/AddressDescriptorKeyFactory.h"

#include "base/StackRelease.h"

#include "bc_base/BinaryUtils.h"
#include "bc_base/AddressDescriptor.h"

#include "bc_trx/AbstractUtxo.h"

namespace codablecash {

AddressDescriptorKey::AddressDescriptorKey() {
	this->desc = nullptr;
	this->utxo = nullptr;
}

AddressDescriptorKey::AddressDescriptorKey(const AddressDescriptor* desc) {
	this->desc = dynamic_cast<AddressDescriptor*>(desc->copyData());
	this->utxo = nullptr;
}

AddressDescriptorKey::~AddressDescriptorKey() {
	delete this->desc;
	delete this->utxo;
}

int AddressDescriptorKey::binarySize() const {
	BinaryUtils::checkNotNull(this->desc);

	int size = sizeof(uint32_t);
	size += this->desc->binarySize();

	return size;
}

void AddressDescriptorKey::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->desc);

	out->putInt(AddressDescriptorKeyFactory::ADDRESS_DESC_KEY);

	this->desc->toBinary(out);
}

AddressDescriptorKey* AddressDescriptorKey::fromBinary(ByteBuffer *in) {
	AddressDescriptor* desc = AddressDescriptor::createFromBinary(in); __STP(desc);
	BinaryUtils::checkNotNull(desc);

	return new AddressDescriptorKey(desc);
}

int AddressDescriptorKey::compareTo(const AbstractBtreeKey *key) const noexcept {
	if(key->isInfinity()){
		return -1;
	}
	else if(key->isNull()){
		return 1;
	}

	const AddressDescriptorKey* other = dynamic_cast<const AddressDescriptorKey*>(key);
	assert(other != nullptr);

	return this->desc->compareTo(other->desc);
}

AbstractBtreeKey* AddressDescriptorKey::clone() const noexcept {
	return new AddressDescriptorKey(this->desc);
}

void AddressDescriptorKey::setUtxo(const AbstractUtxo *utxo) noexcept {
	delete this->utxo;
	this->utxo = dynamic_cast<AbstractUtxo*>(utxo->copyData());
}

} /* namespace codablecash */
