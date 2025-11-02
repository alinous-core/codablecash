/*
 * CoinbaseTransactionWalletHandler.h
 *
 *  Created on: Sep 9, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_COINBASETRANSACTIONWALLETHANDLER_H_
#define BC_WALLET_TRX_COINBASETRANSACTIONWALLETHANDLER_H_

#include "bc_wallet_trx/AbstractWalletTransactionHandler.h"

namespace codablecash {

class CoinbaseTransactionWalletHandler : public AbstractWalletTransactionHandler {
public:
	explicit CoinbaseTransactionWalletHandler(WalletAccount* account);
	virtual ~CoinbaseTransactionWalletHandler();

	virtual void importTransaction(const AbstractBlockchainTransaction *__trx);
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_COINBASETRANSACTIONWALLETHANDLER_H_ */
