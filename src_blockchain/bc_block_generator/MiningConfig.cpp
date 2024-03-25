/*
 * MiningConfig.cpp
 *
 *  Created on: 2023/03/29
 *      Author: iizuka
 */

#include "bc_block_generator/MiningConfig.h"

#include "bc_base/AddressDescriptor.h"

namespace codablecash {

MiningConfig::MiningConfig(const MiningConfig &inst) {
	this->addressDesc = inst.addressDesc != nullptr ? dynamic_cast<AddressDescriptor*>(inst.addressDesc->copyData()) : nullptr;
}

MiningConfig::MiningConfig() {
	this->addressDesc = nullptr;
}

MiningConfig::~MiningConfig() {
	delete this->addressDesc;
}

void MiningConfig::setAddressDescriptor(const AddressDescriptor *addressDesc) noexcept {
	delete this->addressDesc;
	this->addressDesc = dynamic_cast<AddressDescriptor*>(addressDesc->copyData());
}

} /* namespace codablecash */
