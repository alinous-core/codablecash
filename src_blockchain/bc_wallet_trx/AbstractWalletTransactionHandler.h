/*
 * AbstractWalletTransactionHandler.h
 *
 *  Created on: 2023/02/25
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_ABSTRACTWALLETTRANSACTIONHANDLER_H_
#define BC_WALLET_TRX_ABSTRACTWALLETTRANSACTIONHANDLER_H_

namespace codablecash {

class WalletAccount;
class AbstractBlockchainTransaction;

class AbstractWalletTransactionHandler {
public:
	explicit AbstractWalletTransactionHandler(WalletAccount* account);
	virtual ~AbstractWalletTransactionHandler();

	virtual void importTransaction(const AbstractBlockchainTransaction *trx) = 0;

protected:
	WalletAccount* account;
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_ABSTRACTWALLETTRANSACTIONHANDLER_H_ */
