/*
 * NetworkTransactionHandler.h
 *
 *  Created on: Jul 29, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_NETWORKTRANSACTIONHANDLER_H_
#define BC_WALLET_NET_NETWORKTRANSACTIONHANDLER_H_

#include "base/ArrayList.h"
#include <cstdint>

using alinous::ArrayList;

namespace codablecash {

class NodeIdentifierSource;
class BalanceUnit;
class AddressDescriptor;
class IWalletDataEncoder;
class NetworkWallet;
class AbstractClientRequestCommand;
class ISystemLogger;
class AbstractCommandResponse;
class NodeIdentifier;
class AbstractBlockchainTransaction;
class TransactionId;
class AbstractInterChainCommunicationTansaction;
class AbstractSmartcontractTransaction;


class NetworkTransactionHandler {
public:
	NetworkTransactionHandler(int accountIndex, NetworkWallet* netWallet, ISystemLogger* logger);
	virtual ~NetworkTransactionHandler();

	void sendRegisterVotePoolTransaction(const BalanceUnit& feeRate, const IWalletDataEncoder* encoder);
	void sendRegisterTicketTransaction(const NodeIdentifier *nodeId, const BalanceUnit& stakeAmount, const BalanceUnit& feeRate, const IWalletDataEncoder* encoder);

	void sendInterChainCommunicationTransaction(const AbstractInterChainCommunicationTansaction* icctrx);
	void sendSmartcontractTansaction(const AbstractSmartcontractTransaction* sctrx);

	ArrayList<NodeIdentifier>* listStakingNodeIds();

	const AbstractBlockchainTransaction* getLastTransaction() const noexcept {
		return this->lastTrx;
	}

	ArrayList<AbstractBlockchainTransaction>* fetchMempoolTransactions();


	uint8_t getTransactionStoreStatus(const TransactionId* trxId) const noexcept;

	uint64_t getFinalizedHeight() const;

private:
	void broadcastTransaction(const AbstractClientRequestCommand* command);

	void setLastTransaction(const AbstractBlockchainTransaction* trx) noexcept;

private:
	int accountIndex;
	NetworkWallet* netWallet;

	ISystemLogger* logger;

	AbstractBlockchainTransaction* lastTrx;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_NETWORKTRANSACTIONHANDLER_H_ */
