/*
 * RevokeMissVotedTicketWalletHandler.cpp
 *
 *  Created on: Sep 9, 2025
 *      Author: iizuka
 */

#include "bc_wallet_trx/RevokeMissVotedTicketWalletHandler.h"

namespace codablecash {

RevokeMissVotedTicketWalletHandler::RevokeMissVotedTicketWalletHandler(WalletAccount* account)
		: AbstractWalletTransactionHandler(account) {


}

RevokeMissVotedTicketWalletHandler::~RevokeMissVotedTicketWalletHandler() {

}

void RevokeMissVotedTicketWalletHandler::importTransaction(const AbstractBlockchainTransaction *__trx) {
	__importTransaction(__trx);
}

} /* namespace codablecash */
