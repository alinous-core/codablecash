/*
 * GenesisBalanceConfig.cpp
 *
 *  Created on: 2023/08/21
 *      Author: iizuka
 */

#include "bc_network_instance/GenesisBalanceConfig.h"

#include "bc_base/AddressDescriptor.h"

namespace codablecash {

GenesisBalance::GenesisBalance(const GenesisBalance &inst) : amount(inst.amount) {
	this->addressDesc = inst.addressDesc != nullptr ? dynamic_cast<AddressDescriptor*>(inst.addressDesc->copyData()) : nullptr;
}

GenesisBalance::GenesisBalance() : amount(0L) {
	this->addressDesc = nullptr;
}

GenesisBalance::~GenesisBalance() {
	delete this->addressDesc;
}

void GenesisBalance::init(const BalanceUnit amount, const AddressDescriptor *desc) noexcept {
	this->amount = amount;

	delete this->addressDesc;
	this->addressDesc = new AddressDescriptor(*desc);
}

GenesisBalanceConfig::GenesisBalanceConfig(const GenesisBalanceConfig &inst) {
	this->list = new ArrayList<GenesisBalance>();

	int maxLoop = inst.list->size();
	for(int i = 0; i != maxLoop; ++i){
		GenesisBalance* balance = inst.list->get(i);

		this->list->addElement(new GenesisBalance(*balance));
	}
}

GenesisBalanceConfig::GenesisBalanceConfig() {
	this->list = new ArrayList<GenesisBalance>();
}

GenesisBalanceConfig::~GenesisBalanceConfig() {
	this->list->deleteElements();
	delete this->list;
}

void GenesisBalanceConfig::addBalance(const BalanceUnit amount, const AddressDescriptor *desc) noexcept {
	GenesisBalance* bl = new GenesisBalance();
	bl->init(amount, desc);

	this->list->addElement(bl);
}

} /* namespace codablecash */
