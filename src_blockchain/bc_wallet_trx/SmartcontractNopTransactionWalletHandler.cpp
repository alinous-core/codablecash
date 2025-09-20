/*
 * SmartcontractNopTransactionWalletHandler.cpp
 *
 *  Created on: Sep 14, 2025
 *      Author: iizuka
 */

#include "bc_wallet_trx/SmartcontractNopTransactionWalletHandler.h"
#include "bc_wallet_trx_repo/WalletAccountTrxRepository.h"

#include "bc_wallet/WalletAccount.h"

namespace codablecash {

SmartcontractNopTransactionWalletHandler::SmartcontractNopTransactionWalletHandler(WalletAccount* account)
		: AbstractWalletTransactionHandler(account) {

}

SmartcontractNopTransactionWalletHandler::~SmartcontractNopTransactionWalletHandler() {

}

void SmartcontractNopTransactionWalletHandler::importTransaction(const AbstractBlockchainTransaction *trx) {
	WalletAccountTrxRepository* trxRepo = this->account->getWalletAccountTrxRepository();
	trxRepo->importTransaction(trx);
}

} /* namespace codablecash */
