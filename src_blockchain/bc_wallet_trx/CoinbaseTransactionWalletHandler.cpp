/*
 * CoinbaseTransactionWalletHandler.cpp
 *
 *  Created on: Sep 9, 2025
 *      Author: iizuka
 */

#include "bc_wallet_trx/CoinbaseTransactionWalletHandler.h"

#include "bc_block_body/CoinbaseTransaction.h"

#include "bc_trx/AbstractUtxoReference.h"
#include "bc_trx/AbstractUtxo.h"

#include "bc_wallet_trx_repo/WalletAccountTrxRepository.h"

#include "bc_wallet/WalletAccount.h"

#include "base/StackRelease.h"


namespace codablecash {

CoinbaseTransactionWalletHandler::CoinbaseTransactionWalletHandler(WalletAccount* account)
		: AbstractWalletTransactionHandler(account) {

}

CoinbaseTransactionWalletHandler::~CoinbaseTransactionWalletHandler() {

}

void CoinbaseTransactionWalletHandler::importTransaction(const AbstractBlockchainTransaction *__trx) {
	__importTransaction(__trx);
}

} /* namespace codablecash */
