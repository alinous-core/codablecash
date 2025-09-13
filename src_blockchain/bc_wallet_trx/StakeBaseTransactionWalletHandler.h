/*
 * StakeBaseTransactionWalletHandler.h
 *
 *  Created on: Sep 9, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_STAKEBASETRANSACTIONWALLETHANDLER_H_
#define BC_WALLET_TRX_STAKEBASETRANSACTIONWALLETHANDLER_H_

#include "bc_wallet_trx/AbstractWalletTransactionHandler.h"

namespace codablecash {

class StakeBaseTransactionWalletHandler : public AbstractWalletTransactionHandler {
public:
	explicit StakeBaseTransactionWalletHandler(WalletAccount* account);
	virtual ~StakeBaseTransactionWalletHandler();

	virtual void importTransaction(const AbstractBlockchainTransaction *__trx);
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_STAKEBASETRANSACTIONWALLETHANDLER_H_ */
