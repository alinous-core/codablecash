/*
 * FetchHeaderTransactionsWorker.cpp
 *
 *  Created on: Aug 20, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_cmd/FetchHeaderTransactionsWorker.h"

#include "bc_p2p_cmd_client/ClientFetchHeaderTransactionsCommand.h"
#include "bc_p2p_cmd_client/ClientFetchHeaderTransactionsCommandResponse.h"
#include "bc_p2p_cmd_client/ClientBlockHeaderTransferData.h"

#include "bc_block/BlockHeaderId.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_wallet_net/WalletNetworkManager.h"

#include "bc/ExceptionThrower.h"
#include "bc/ISystemLogger.h"

#include "bc_p2p/BlockchainNodeHandshakeException.h"
#include "bc_p2p/StackHandshakeReleaser.h"
#include "bc_p2p/ClientNodeHandshake.h"

#include "base/StackRelease.h"


namespace codablecash {

FetchHeaderTransactionsWorker::FetchHeaderTransactionsWorker(uint16_t zone, const BlockHeaderId* headerId, uint64_t height
		, const NodeIdentifier* nodeId, WalletNetworkManager* networkManager, ISystemLogger* logger, const UnicodeString* name)
		: AbstractSyncWorkerThread(logger, name) {
	this->zone = zone;
	this->headerId = dynamic_cast<BlockHeaderId*>(headerId->copyData());
	this->height = height;

	this->nodeId = dynamic_cast<NodeIdentifier*>(nodeId->copyData());
	this->networkManager = networkManager;
	this->response = nullptr;
}

FetchHeaderTransactionsWorker::~FetchHeaderTransactionsWorker() {
	delete this->headerId;
	delete this->nodeId;
	this->networkManager = nullptr;

	delete this->response;
}

void FetchHeaderTransactionsWorker::doProcess() {
	const NodeIdentifierSource* source = this->networkManager->getNodeIdentifierSource();

	ClientFetchHeaderTransactionsCommand command;
	command.setZone(this->zone);
	command.setHeight(this->height);
	command.setBlockHeaderId(this->headerId);

	command.sign(source);

	ClientNodeHandshake* handshake = this->networkManager->getClientHandshakeByNodeId(this->nodeId);
	ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(handshake == nullptr, L"Node connection has alrealy closed.", __FILE__, __LINE__);

	StackHandshakeReleaser __releaser(handshake);

	AbstractCommandResponse* res = handshake->sendCommnad(&command); __STP(res);
	ClientFetchHeaderTransactionsCommandResponse* fetchResponse = dynamic_cast<ClientFetchHeaderTransactionsCommandResponse*>(res);

	ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(fetchResponse == nullptr, L"Respose error.", __FILE__, __LINE__);

	{
		UnicodeString message(L"Accept Response : ");
		UnicodeString* responseStr = res->toString(); __STP(responseStr);
		message.append(responseStr);

		this->logger->debugLog(ISystemLogger::DEBUG_NODE_TRANSFER_RESPONSE, &message, __FILE__, __LINE__);
	}

	const ClientBlockHeaderTransferData* headerData = fetchResponse->getTransferData();
	headerData->validateVoteTransactionsData();
	headerData->validateTransactions();

	__STP_MV(res);
	this->response = fetchResponse;
}

} /* namespace codablecash */
