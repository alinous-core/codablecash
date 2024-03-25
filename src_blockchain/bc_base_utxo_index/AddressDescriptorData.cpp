/*
 * AddressDescriptorData.cpp
 *
 *  Created on: 2023/02/24
 *      Author: iizuka
 */

#include "bc_base_utxo_index/AddressDescriptorData.h"

#include "bc_base/AddressDescriptor.h"
#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

namespace codablecash {

AddressDescriptorData::AddressDescriptorData(const AddressDescriptor* desc) {
	this->desc = dynamic_cast<AddressDescriptor*>(desc->copyData());
}

AddressDescriptorData::~AddressDescriptorData() {
	delete this->desc;
}

int AddressDescriptorData::binarySize() const {
	int total = this->desc->binarySize();

	return total;
}

void AddressDescriptorData::toBinary(ByteBuffer *out) const {
	this->desc->toBinary(out);
}

AddressDescriptorData* AddressDescriptorData::fromBinary(ByteBuffer *in) {
	AddressDescriptor* desc = AddressDescriptor::createFromBinary(in); __STP(desc);

	BinaryUtils::checkNotNull(desc);

	return new AddressDescriptorData(desc);
}

IBlockObject* AddressDescriptorData::copyData() const noexcept {
	return new AddressDescriptorData(this->desc);
}

} /* namespace codablecash */
