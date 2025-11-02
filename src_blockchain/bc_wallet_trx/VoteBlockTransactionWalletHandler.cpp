/*
 * VoteBlockTransactionWalletHandler.cpp
 *
 *  Created on: Sep 9, 2025
 *      Author: iizuka
 */

#include "bc_wallet_trx/VoteBlockTransactionWalletHandler.h"

namespace codablecash {

VoteBlockTransactionWalletHandler::VoteBlockTransactionWalletHandler(WalletAccount* account)
		: AbstractWalletTransactionHandler(account) {


}

VoteBlockTransactionWalletHandler::~VoteBlockTransactionWalletHandler() {

}

void VoteBlockTransactionWalletHandler::importTransaction(const AbstractBlockchainTransaction *__trx) {
	__importTransaction(__trx);
}

} /* namespace codablecash */
