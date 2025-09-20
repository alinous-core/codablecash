/*
 * IccNopTransactionWalletHandler.cpp
 *
 *  Created on: Sep 14, 2025
 *      Author: iizuka
 */

#include "bc_wallet_trx/IccNopTransactionWalletHandler.h"
#include "bc_wallet_trx_repo/WalletAccountTrxRepository.h"

#include "bc_wallet/WalletAccount.h"

namespace codablecash {

IccNopTransactionWalletHandler::IccNopTransactionWalletHandler(WalletAccount* account)
		: AbstractWalletTransactionHandler(account) {

}

IccNopTransactionWalletHandler::~IccNopTransactionWalletHandler() {

}

void IccNopTransactionWalletHandler::importTransaction(const AbstractBlockchainTransaction *trx) {
	WalletAccountTrxRepository* trxRepo = this->account->getWalletAccountTrxRepository();
	trxRepo->importTransaction(trx);
}

} /* namespace codablecash */
