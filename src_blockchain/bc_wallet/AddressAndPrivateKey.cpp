/*
 * AddressAndPrivateKey.cpp
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */

#include "bc_wallet/AddressAndPrivateKey.h"

#include "ecda/ScPrivateKey.h"

#include "bc_base/BalanceAddress.h"

namespace codablecash {

AddressAndPrivateKey::AddressAndPrivateKey() {
	this->privateKey = nullptr;
	this->address = nullptr;
}

AddressAndPrivateKey::~AddressAndPrivateKey() {
	delete this->privateKey;
	delete this->address;
}

void AddressAndPrivateKey::setPrivateKey(ScPrivateKey *privateKey) noexcept {
	delete this->privateKey;
	this->privateKey = privateKey;
}

void AddressAndPrivateKey::setBalanceAddress(BalanceAddress *address) noexcept {
	delete this->address;
	this->address = address;
}

} /* namespace codablecash */
