/*
 * InstanceSpace.cpp
 *
 *  Created on: Nov 28, 2025
 *      Author: iizuka
 */

#include "smartcontract_cache/InstanceSpace.h"

#include "smartcontract_instance/ModularSmartcontractInstance.h"


namespace codablecash {

InstanceSpace::InstanceSpace(const SmartcontractInstanceAddress *instAddress, ModularSmartcontractInstance* instance) {
	this->instance = instance;
	this->instance->setSmartcontractInstanceAddress(instAddress);

	this->ref = 0;
}

InstanceSpace::~InstanceSpace() {
	delete this->instance;
}

bool InstanceSpace::isDeletable() const noexcept {
	return this->ref == 0;
}

void InstanceSpace::beforeCacheOut() {
	this->instance->cleanDbRoot();
}

const SmartcontractInstanceAddress* InstanceSpace::getSmartContractInstanceAddress() const noexcept {
	return this->instance->getSmartContractInstanceAddress();
}

} /* namespace codablecash */
