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
class ITransactionBuilderContext;

class RegisterTicketTransactionWalletHandler : public AbstractWalletTransactionHandler {
public:
	explicit RegisterTicketTransactionWalletHandler(WalletAccount* account);
	virtual ~RegisterTicketTransactionWalletHandler();

	RegisterTicketTransaction* createTransaction(
			const NodeIdentifier *nodeId, const BalanceUnit& stakeAmount, const BalanceUnit& feeRate, const AddressDescriptor* ticketReturnaddressDesc,
			const IWalletDataEncoder *encoder, ITransactionBuilderContext* context);

	virtual void importTransaction(const AbstractBlockchainTransaction *__trx);

private:
	void collectUtxoRefs(RegisterTicketTransaction* trx, BalanceUnit& amount, const BalanceUnit& feeRate
			, IUtxoCollector *collector, ArrayUtxoFinder *utxoFinder, HdWalleMuSigSignerProvidor *musigProvidor, const IWalletDataEncoder* encoder);
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_REGISTERTICKETTRANSACTIONWALLETHANDLER_H_ */
