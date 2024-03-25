/*
 * GenesisTransactionHandler.h
 *
 *  Created on: 2023/02/25
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_GENESISTRANSACTIONHANDLER_H_
#define BC_WALLET_TRX_GENESISTRANSACTIONHANDLER_H_

#include "bc_wallet_trx/AbstractWalletTransactionHandler.h"

namespace codablecash {

class BalanceUnit;
class GenesisTransaction;

class GenesisTransactionHandler : public AbstractWalletTransactionHandler {
public:
	explicit GenesisTransactionHandler(WalletAccount* account);
	virtual ~GenesisTransactionHandler();

	GenesisTransaction* createTransaction(const BalanceUnit amount, int pos);
	virtual void importTransaction(const AbstractBlockchainTransaction *trx);
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_GENESISTRANSACTIONHANDLER_H_ */
