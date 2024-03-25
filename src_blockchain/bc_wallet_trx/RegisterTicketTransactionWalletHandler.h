/*
 * RegisterTicketTransactionWalletHandler.h
 *
 *  Created on: 2023/04/27
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_REGISTERTICKETTRANSACTIONWALLETHANDLER_H_
#define BC_WALLET_TRX_REGISTERTICKETTRANSACTIONWALLETHANDLER_H_

#include "bc_wallet_trx/AbstractWalletTransactionHandler.h"

namespace codablecash {

class RegisterTicketTransaction;
class BalanceUnit;
class NodeIdentifier;
class AddressDescriptor;
class IWalletDataEncoder;

class RegisterTicketTransactionWalletHandler : public AbstractWalletTransactionHandler {
public:
	explicit RegisterTicketTransactionWalletHandler(WalletAccount* account);
	virtual ~RegisterTicketTransactionWalletHandler();

	RegisterTicketTransaction* createTransaction(
			const NodeIdentifier *nodeId, const BalanceUnit stakeAmount, const BalanceUnit feeRate, const AddressDescriptor* addressDesc,
			const IWalletDataEncoder *encoder);
	virtual void importTransaction(const AbstractBlockchainTransaction *__trx);
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_REGISTERTICKETTRANSACTIONWALLETHANDLER_H_ */
