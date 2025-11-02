/*
 * StakeBaseTransactionWalletHandler.cpp
 *
 *  Created on: Sep 9, 2025
 *      Author: iizuka
 */

#include "bc_wallet_trx/StakeBaseTransactionWalletHandler.h"

namespace codablecash {

StakeBaseTransactionWalletHandler::StakeBaseTransactionWalletHandler(WalletAccount* account)
		: AbstractWalletTransactionHandler(account) {

}

StakeBaseTransactionWalletHandler::~StakeBaseTransactionWalletHandler() {

}

void StakeBaseTransactionWalletHandler::importTransaction(const AbstractBlockchainTransaction *__trx) {
	__importTransaction(__trx);
}

} /* namespace codablecash */
