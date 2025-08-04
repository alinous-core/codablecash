/*
 * AbstractWalletTransactionHandler.cpp
 *
 *  Created on: 2023/02/25
 *      Author: iizuka
 */

#include "bc_wallet_trx/AbstractWalletTransactionHandler.h"

#include "bc_wallet/WalletAccount.h"

namespace codablecash {

AbstractWalletTransactionHandler::AbstractWalletTransactionHandler(WalletAccount* account) {
	this->account = account;
}

AbstractWalletTransactionHandler::~AbstractWalletTransactionHandler() {
	this->account = nullptr;
}



} /* namespace codablecash */
