/*
 * BalanceTransactionWalletHandler.h
 *
 *  Created on: 2023/02/25
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_BALANCETRANSACTIONWALLETHANDLER_H_
#define BC_WALLET_TRX_BALANCETRANSACTIONWALLETHANDLER_H_

#include "bc_wallet_trx/AbstractWalletTransactionHandler.h"

namespace codablecash {

class BalanceTransferTransaction;
class AddressDescriptor;
class BalanceUnit;
class IWalletDataEncoder;

class BalanceTransactionWalletHandler: public AbstractWalletTransactionHandler {
public:
	explicit BalanceTransactionWalletHandler(WalletAccount* account);
	virtual ~BalanceTransactionWalletHandler();

	BalanceTransferTransaction* createTransaction(const AddressDescriptor* dest, const BalanceUnit amount
			, const BalanceUnit feeRate, bool feeIncluded, const IWalletDataEncoder* encoder);
	virtual void importTransaction(const AbstractBlockchainTransaction *trx);
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_BALANCETRANSACTIONWALLETHANDLER_H_ */
