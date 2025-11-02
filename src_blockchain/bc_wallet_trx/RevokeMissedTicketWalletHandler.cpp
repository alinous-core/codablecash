/*
 * RevokeMissedTicketWalletHandler.cpp
 *
 *  Created on: Sep 9, 2025
 *      Author: iizuka
 */

#include "bc_wallet_trx/RevokeMissedTicketWalletHandler.h"

namespace codablecash {

RevokeMissedTicketWalletHandler::RevokeMissedTicketWalletHandler(WalletAccount* account)
		: AbstractWalletTransactionHandler(account) {


}

RevokeMissedTicketWalletHandler::~RevokeMissedTicketWalletHandler() {

}

void RevokeMissedTicketWalletHandler::importTransaction(const AbstractBlockchainTransaction *__trx) {
	__importTransaction(__trx);
}

} /* namespace codablecash */
