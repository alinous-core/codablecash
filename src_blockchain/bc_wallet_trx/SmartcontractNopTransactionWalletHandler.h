/*
 * SmartcontractNopTransactionWalletHandler.h
 *
 *  Created on: Sep 14, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_SMARTCONTRACTNOPTRANSACTIONWALLETHANDLER_H_
#define BC_WALLET_TRX_SMARTCONTRACTNOPTRANSACTIONWALLETHANDLER_H_

#include "bc_wallet_trx/AbstractWalletTransactionHandler.h"

namespace codablecash {

class SmartcontractNopTransactionWalletHandler : public AbstractWalletTransactionHandler {
public:
	SmartcontractNopTransactionWalletHandler(WalletAccount* account);
	virtual ~SmartcontractNopTransactionWalletHandler();

	virtual void importTransaction(const AbstractBlockchainTransaction *trx);
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_SMARTCONTRACTNOPTRANSACTIONWALLETHANDLER_H_ */
