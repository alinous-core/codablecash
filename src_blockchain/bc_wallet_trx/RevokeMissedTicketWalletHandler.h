/*
 * RevokeMissedTicketWalletHandler.h
 *
 *  Created on: Sep 9, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_REVOKEMISSEDTICKETWALLETHANDLER_H_
#define BC_WALLET_TRX_REVOKEMISSEDTICKETWALLETHANDLER_H_

#include "bc_wallet_trx/AbstractWalletTransactionHandler.h"

namespace codablecash {

class RevokeMissedTicketWalletHandler : public AbstractWalletTransactionHandler {
public:
	explicit RevokeMissedTicketWalletHandler(WalletAccount* account);
	virtual ~RevokeMissedTicketWalletHandler();

	virtual void importTransaction(const AbstractBlockchainTransaction *__trx);
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_REVOKEMISSEDTICKETWALLETHANDLER_H_ */
