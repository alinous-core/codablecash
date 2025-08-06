/*
 * NetworkTransactionHandler.h
 *
 *  Created on: Jul 29, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_NETWORKTRANSACTIONHANDLER_H_
#define BC_WALLET_NET_NETWORKTRANSACTIONHANDLER_H_

namespace codablecash {

class NodeIdentifierSource;
class BalanceUnit;
class AddressDescriptor;
class IWalletDataEncoder;
class NetworkWallet;
class AbstractClientRequestCommand;
class ISystemLogger;

class NetworkTransactionHandler {
public:
	NetworkTransactionHandler(int accountIndex, NetworkWallet* netWallet, ISystemLogger* logger);
	virtual ~NetworkTransactionHandler();

	void sendRegisterVotePoolTransaction(const BalanceUnit& feeRate, const IWalletDataEncoder* encoder);

	void listSTakingNodeIds();


private:
	void broadcastTransaction(const AbstractClientRequestCommand* command);

private:
	int accountIndex;
	NetworkWallet* netWallet;

	ISystemLogger* logger;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_NETWORKTRANSACTIONHANDLER_H_ */
