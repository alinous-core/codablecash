/*
 * RegisterVotePoolTransactionWalletHandler.h
 *
 *  Created on: 2023/04/15
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_REGISTERVOTEPOOLTRANSACTIONWALLETHANDLER_H_
#define BC_WALLET_TRX_REGISTERVOTEPOOLTRANSACTIONWALLETHANDLER_H_

#include "bc_wallet_trx/AbstractWalletTransactionHandler.h"

namespace codablecash {

class RegisterVotePoolTransaction;
class NodeIdentifier;
class BalanceUnit;
class IWalletDataEncoder;
class AddressDescriptor;
class NodeIdentifierSource;
class ITransactionBuilderContext;

class RegisterVotePoolTransactionWalletHandler : public AbstractWalletTransactionHandler {
public:
	explicit RegisterVotePoolTransactionWalletHandler(WalletAccount* account);
	virtual ~RegisterVotePoolTransactionWalletHandler();

	RegisterVotePoolTransaction* createTransaction(
			const NodeIdentifierSource *source, const BalanceUnit& feeRate, const AddressDescriptor* addressDesc,
			const IWalletDataEncoder *encoder, ITransactionBuilderContext* context);

	virtual void importTransaction(const AbstractBlockchainTransaction *trx);


private:
	void collectUtxoRefs(RegisterVotePoolTransaction* trx, const BalanceUnit& amount, const BalanceUnit& feeRate
			, IUtxoCollector *collector, ArrayUtxoFinder *utxoFinder, HdWalleMuSigSignerProvidor *musigProvidor, const IWalletDataEncoder* encoder);
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_REGISTERVOTEPOOLTRANSACTIONWALLETHANDLER_H_ */
