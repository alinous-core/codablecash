/*
 * IccNopTransactionWalletHandler.h
 *
 *  Created on: Sep 14, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_ICCNOPTRANSACTIONWALLETHANDLER_H_
#define BC_WALLET_TRX_ICCNOPTRANSACTIONWALLETHANDLER_H_

#include "bc_wallet_trx/AbstractWalletTransactionHandler.h"

namespace codablecash {

class IccNopTransactionWalletHandler : public AbstractWalletTransactionHandler {
public:
	IccNopTransactionWalletHandler(WalletAccount* account);
	virtual ~IccNopTransactionWalletHandler();

	virtual void importTransaction(const AbstractBlockchainTransaction *trx);
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_ICCNOPTRANSACTIONWALLETHANDLER_H_ */
