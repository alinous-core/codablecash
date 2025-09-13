/*
 * VoteBlockTransactionWalletHandler.h
 *
 *  Created on: Sep 9, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_VOTEBLOCKTRANSACTIONWALLETHANDLER_H_
#define BC_WALLET_TRX_VOTEBLOCKTRANSACTIONWALLETHANDLER_H_

#include "bc_wallet_trx/AbstractWalletTransactionHandler.h"

namespace codablecash {

class VoteBlockTransactionWalletHandler : public AbstractWalletTransactionHandler {
public:
	explicit VoteBlockTransactionWalletHandler(WalletAccount* account);
	virtual ~VoteBlockTransactionWalletHandler();

	virtual void importTransaction(const AbstractBlockchainTransaction *__trx);
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_VOTEBLOCKTRANSACTIONWALLETHANDLER_H_ */
