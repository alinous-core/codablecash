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
#include "bc_finalizer_trx/RegisterTicketTransaction.h"

#include "bc_p2p_cmd_client/SendTransactionClientCommand.h"

#include "bc_network/NodeIdentifierSource.h"
#include "bc_network/NodeIdentifier.h"

#include "bc_p2p/StackHandshakeReleaser.h"
#include "bc_p2p/ClientNodeHandshake.h"

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "bc/ISystemLogger.h"

#include "bc_p2p_cmd_client/ClientListStakingNodeIdsCommand.h"
#include "bc_p2p_cmd_client/ClientListStakingNodeIdsCommandResponse.h"
#include "bc_p2p_cmd_client/ClientFetchMempoolTrxCommandResponse.h"
#include "bc_p2p_cmd_client/ClientFetchMempoolTrxCommand.h"

#include "bc_network_instance_sync/RamdomNodesSelector.h"

#include "bc_wallet_net_access/RetriableClientAccessContainer.h"
#include "bc_wallet_net_access/ListStakingNodeAccess.h"

#include "bc_trx/AbstractBlockchainTransaction.h"

#include "bc_wallet_net_access/ClientFetchMempoolTrxAccess.h"


namespace codablecash {

NetworkTransactionHandler::NetworkTransactionHandler(int accountIndex, NetworkWallet* netWallet, ISystemLogger* logger) {
	this->accountIndex = accountIndex;
	this->netWallet = netWallet;
	this->logger = logger;
	this->lastTrx = nullptr;
}

NetworkTransactionHandler::~NetworkTransactionHandler() {
	this->netWallet = nullptr;
	this->logger = nullptr;

	delete this->lastTrx;
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

		bool bl = trx->checkUtxoRefs();
		assert(bl);
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

	setLastTransaction(trx);
}

void NetworkTransactionHandler::sendRegisterTicketTransaction(const NodeIdentifier *nodeId, const BalanceUnit &stakeAmount,
		const BalanceUnit &feeRate, const IWalletDataEncoder *encoder) {
	NetworkWalletData* data = this->netWallet->getWalletData();
	ConcurrentGate* lock = data->getLock();

	RegisterTicketTransaction* trx = nullptr;
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

		trx = account->createRegisterTicketTransaction(nodeId, stakeAmount, feeRate, desc, encoder, &context);

		bool bl = trx->checkUtxoRefs();
		assert(bl);
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

	setLastTransaction(trx);
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

ArrayList<NodeIdentifier>* NetworkTransactionHandler::listStakingNodeIds() {
	WalletNetworkManager* networkManager = this->netWallet->getWalletNetworkManager();

	const NodeIdentifierSource* source = networkManager->getNodeIdentifierSource();

	NetworkWalletData* data = this->netWallet->getWalletData();
	uint16_t zone = data->getDefaultZone();

	ClientListStakingNodeIdsCommand command;
	command.setZone(zone);

	command.sign(source);

	ListStakingNodeAccess access(&command);
	RetriableClientAccessContainer retriable(&access, this->logger, networkManager);

	retriable.clientAccess();

	const ClientListStakingNodeIdsCommandResponse* response = access.getResponse();
	const ArrayList<NodeIdentifier>* list = response->getNodeIdentifierList();

	ArrayList<NodeIdentifier>* ret = new ArrayList<NodeIdentifier>();

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		const NodeIdentifier* nodeId = list->get(i);

		ret->addElement(dynamic_cast<NodeIdentifier*>(nodeId->copyData()));
	}

	return ret;
}

ArrayList<AbstractBlockchainTransaction>* NetworkTransactionHandler::fetchMempoolTransactions() {
	WalletNetworkManager* networkManager = this->netWallet->getWalletNetworkManager();

	const NodeIdentifierSource* source = networkManager->getNodeIdentifierSource();
	NetworkWalletData* data = this->netWallet->getWalletData();
	uint16_t zone = data->getDefaultZone();

	ClientFetchMempoolTrxCommand command;
	command.setZone(zone);

	command.sign(source);

	ClientFetchMempoolTrxAccess access(&command);
	RetriableClientAccessContainer retriable(&access, this->logger, networkManager);

	retriable.clientAccess();

	const ClientFetchMempoolTrxCommandResponse* response = access.getResponse();

	ArrayList<AbstractBlockchainTransaction>* ret = new ArrayList<AbstractBlockchainTransaction>();
	const ArrayList<AbstractBlockchainTransaction>* list = response->getList();

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		const AbstractBlockchainTransaction* t = list->get(i);

		ret->addElement(dynamic_cast<AbstractBlockchainTransaction*>(t->copyData()));
	}

	return ret;
}

void NetworkTransactionHandler::setLastTransaction(const AbstractBlockchainTransaction *trx) noexcept {
	delete this->lastTrx;
	this->lastTrx = dynamic_cast<AbstractBlockchainTransaction*>(trx->copyData());
}

/**
 * ManagedUtxoCacheRecord::NONE
 * @param trxId
 * @return
 */
uint8_t NetworkTransactionHandler::getTransactionStoreStatus(const TransactionId *trxId) const noexcept {
	NetworkWalletData* data = this->netWallet->getWalletData();

	ManagementAccountsCollection* collection = data->getManagementAccountsCollection();
	uint8_t storeType = collection->getTransactionStoreStatus(trxId);

	return storeType;
}

uint64_t NetworkTransactionHandler::getFinalizedHeight() const {
	NetworkWalletData* data = this->netWallet->getWalletData();

	return data->getFinalizedHeight();
}

} /* namespace codablecash */
