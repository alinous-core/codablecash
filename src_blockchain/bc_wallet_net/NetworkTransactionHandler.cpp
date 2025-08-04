/*
 * NetworkTransactionHandler.cpp
 *
 *  Created on: Jul 29, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net/NetworkTransactionHandler.h"
#include "bc_wallet_net/NetworkWallet.h"
#include "bc_wallet_net/WalletNetworkManager.h"

#include "bc_wallet_net_data/NetworkWalletData.h"

#include "bc_wallet/HdWallet.h"
#include "bc_wallet/WalletAccount.h"
#include "bc_wallet/ChangeAddressStore.h"

#include "bc_wallet_net_data_management/ManagementAccountsCollection.h"

#include "base/StackRelease.h"

#include "base_thread/ConcurrentGate.h"

#include "bc_base/AddressDescriptor.h"

#include "bc_wallet_encoder/IWalletDataEncoder.h"

#include "bc_wallet_trx_base/NetWalletAccountTrxBuilderContext.h"

#include "bc_finalizer_trx/RegisterVotePoolTransaction.h"

#include "bc_p2p_cmd_client/SendTransactionClientCommand.h"

#include "bc_network/NodeIdentifierSource.h"
#include "bc_network/NodeIdentifier.h"

#include "bc_p2p/StackHandshakeReleaser.h"
#include "bc_p2p/ClientNodeHandshake.h"

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "bc/ISystemLogger.h"

namespace codablecash {

NetworkTransactionHandler::NetworkTransactionHandler(int accountIndex, NetworkWallet* netWallet, ISystemLogger* logger) {
	this->accountIndex = accountIndex;
	this->netWallet = netWallet;
	this->logger = logger;
}

NetworkTransactionHandler::~NetworkTransactionHandler() {
	this->netWallet = nullptr;
	this->logger = nullptr;
}

void NetworkTransactionHandler::sendRegisterVotePoolTransaction(const BalanceUnit &feeRate, const IWalletDataEncoder *encoder) {
	NetworkWalletData* data = this->netWallet->getWalletData();
	ConcurrentGate* lock = data->getLock();

	RegisterVotePoolTransaction* trx = nullptr;
	{
		StackReadLock __lock(lock, __FILE__, __LINE__);

		HdWallet* hdWallet = data->getHdWallet();
		ManagementAccountsCollection* managementAccounts = data->getManagementAccountsCollection();
		ManagementAccount* managementAccount = managementAccounts->getMempoolManagementAccount();

		WalletAccount* account = hdWallet->getAccount(this->accountIndex);
		NodeIdentifierSource *source = this->netWallet->getStakingSourceId(encoder); __STP(source);

		ChangeAddressStore* changeAddresses = account->getChangeAddresses();
		AddressDescriptor* desc = changeAddresses->getNextChangeAddress(encoder); __STP(desc);

		NetWalletAccountTrxBuilderContext context(account, encoder, managementAccounts);

		trx = account->createRegisterVotePoolTransaction(source, feeRate, desc, encoder, &context);
	}
	__STP(trx);

	WalletNetworkManager* networkManager = this->netWallet->getWalletNetworkManager();
	const NodeIdentifierSource* nodeIdSource = networkManager->getNodeIdentifierSource();
	NodeIdentifier clientNodeId = nodeIdSource->toNodeIdentifier();

	SendTransactionClientCommand cmd;
	cmd.setTransaction(trx);
	cmd.setNodeIdentifier(&clientNodeId);
	cmd.sign(nodeIdSource);

	broadcastTransaction(&cmd);
}

void NetworkTransactionHandler::broadcastTransaction(const AbstractClientRequestCommand *command) {
	WalletNetworkManager* networkManager = this->netWallet->getWalletNetworkManager();

	ArrayList<NodeIdentifier>* nodeIdList = networkManager->getNodeIdList(); __STP(nodeIdList);
	nodeIdList->setDeleteOnExit();

	int maxLoop = nodeIdList->size();
	for(int i = 0; i != maxLoop; ++i){
		const NodeIdentifier* nodeId = nodeIdList->get(i);

		ClientNodeHandshake* handshake = networkManager->getClientHandshakeByNodeId(nodeId);
		StackHandshakeReleaser __releaser(handshake);

		if(handshake != nullptr){
			AbstractCommandResponse* response = handshake->sendCommnad(command); __STP(response);

			{
				UnicodeString message(L"Accept Response : ");
				UnicodeString* responseStr = response->toString(); __STP(responseStr);
				message.append(responseStr);

				this->logger->debugLog(ISystemLogger::DEBUG_NODE_TRANSFER_RESPONSE, &message, __FILE__, __LINE__);
			}
		}
	}
}

} /* namespace codablecash */
