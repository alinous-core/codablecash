/*
 * AddressDescriptorKeyFactory.cpp
 *
 *  Created on: 2023/02/23
 *      Author: iizuka
 */

#include "bc_base_utxo_index/AddressDescriptorKeyFactory.h"
#include "bc_base_utxo_index/AddressDescriptorKey.h"

namespace codablecash {

AddressDescriptorKeyFactory::AddressDescriptorKeyFactory() {

}

AddressDescriptorKeyFactory::~AddressDescriptorKeyFactory() {

}

AbstractBtreeKey* AddressDescriptorKeyFactory::fromBinary(uint32_t keyType,	ByteBuffer *in) const {
	if(keyType == AddressDescriptorKeyFactory::ADDRESS_DESC_KEY){
		return AddressDescriptorKey::fromBinary(in);
	}

	return BtreeKeyFactory::fromBinary(keyType, in);
}

BtreeKeyFactory* AddressDescriptorKeyFactory::copy() const noexcept {
	return new AddressDescriptorKeyFactory();
}

} /* namespace codablecash */
