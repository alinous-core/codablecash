/*
 * RevokeMissVotedTicketWalletHandler.h
 *
 *  Created on: Sep 9, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_REVOKEMISSVOTEDTICKETWALLETHANDLER_H_
#define BC_WALLET_TRX_REVOKEMISSVOTEDTICKETWALLETHANDLER_H_

#include "bc_wallet_trx/AbstractWalletTransactionHandler.h"

namespace codablecash {

class RevokeMissVotedTicketWalletHandler : public AbstractWalletTransactionHandler {
public:
	explicit RevokeMissVotedTicketWalletHandler(WalletAccount* account);
	virtual ~RevokeMissVotedTicketWalletHandler();

	virtual void importTransaction(const AbstractBlockchainTransaction *__trx);
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_REVOKEMISSVOTEDTICKETWALLETHANDLER_H_ */
