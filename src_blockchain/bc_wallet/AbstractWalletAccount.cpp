/*
 * AbstractWalletAccount.cpp
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */

#include "bc_wallet/AbstractWalletAccount.h"

namespace codablecash {

AbstractWalletAccount::AbstractWalletAccount(uint16_t zone, int accountIndex) {
	this->zone = zone;
	this->accountIndex = accountIndex;
}

AbstractWalletAccount::~AbstractWalletAccount() {

}

} /* namespace codablecash */
