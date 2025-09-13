/*
 * OmittedBlockBodyFixer.cpp
 *
 *  Created on: Sep 4, 2025
 *      Author: iizuka
 */

#include "bc_block_body/OmittedBlockBodyFixer.h"

#include "bc_network/NodeIdentifier.h"
#include "bc_network/NodeIdentifierSource.h"

#include "bc_p2p_processor/P2pRequestProcessor.h"

#include "bc_memorypool/MemoryPool.h"

#include "bc_block/BlockHeaderId.h"

#include "bc_p2p/BlochchainP2pManager.h"
#include "bc_p2p/BlockchainNodeHandshakeException.h"
#include "bc_p2p/StackHandshakeReleaser.h"
#include "bc_p2p/BlockchainNodeHandshake.h"

#include "bc_p2p_cmd_node/DownloadTransactionsNodeCommand.h"
#include "bc_p2p_cmd_node/DownloadTransactionsNodeCommandResponse.h"

#include "bc/ExceptionThrower.h"
#include "bc/ISystemLogger.h"

#include "pubsub/P2pHandshake.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "bc_trx/TransactionId.h"
#include "bc_trx/AbstractBlockchainTransaction.h"


namespace codablecash {

OmittedBlockBodyFixer::OmittedBlockBodyFixer(const NodeIdentifier* nodeId, MemoryPool* memPool, BlochchainP2pManager* p2pManager, const BlockHeaderId* headerId, uint64_t height, NodeIdentifierSource* source) {
	this->nodeId = dynamic_cast<NodeIdentifier*>(nodeId->copyData());
	this->memPool = memPool;
	this->p2pManager = p2pManager;

	this->command = new DownloadTransactionsNodeCommand();
	this->command->setBlockHeaderId(headerId);
	this->command->setHeight(height);

	this->source = dynamic_cast<NodeIdentifierSource*>(source->copyData());

	this->map = new HashMap<TransactionId, AbstractBlockchainTransaction>();
}

OmittedBlockBodyFixer::~OmittedBlockBodyFixer() {
	delete this->nodeId;
	delete this->command;
	delete this->source;

	this->memPool = nullptr;

	Iterator<TransactionId>* it = this->map->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const TransactionId* trxId = it->next();
		AbstractBlockchainTransaction* trx = this->map->get(trxId);

		delete trx;
	}

	delete this->map;
}

MemPoolTransaction* OmittedBlockBodyFixer::begin() {
	return this->memPool->begin();
}

void OmittedBlockBodyFixer::addDownloadTransactionEntry(const DownloadTransactionEntry *entry) {
	this->command->addTrxId(entry);
}

void OmittedBlockBodyFixer::downloadTransactions(ISystemLogger *logger) {
	if(this->command->isEmpty()){
		return;
	}

	// sign
	this->command->sign(this->source);

	BlockchainNodeHandshake* handshake = p2pManager->getNodeHandshakeByNodeId(this->nodeId);
	ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(handshake == nullptr, L"Node connection has alrealy closed.", __FILE__, __LINE__);

	StackHandshakeReleaser __releaser(handshake);

	P2pHandshake* p2pHandshake = handshake->getHandshake();

	DownloadTransactionsNodeCommandResponse* response = __downloadTransactions(this->command, p2pHandshake, logger); __STP(response);

	int maxLoop = response->size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractBlockchainTransaction* trx = response->get(i);
		const TransactionId* trxId = trx->getTransactionId();

		this->map->put(trxId, dynamic_cast<AbstractBlockchainTransaction*>(trx->copyData()));
	}
}

DownloadTransactionsNodeCommandResponse* OmittedBlockBodyFixer::__downloadTransactions(const DownloadTransactionsNodeCommand *command, P2pHandshake *handshake, ISystemLogger *logger) {
	DownloadTransactionsNodeCommandResponse* ret = nullptr;
	{
		ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(handshake == nullptr, L"Node connection has alrealy closed.", __FILE__, __LINE__);

		AbstractCommandResponse* response = handshake->publishCommand(command); __STP(response);
		ret = dynamic_cast<DownloadTransactionsNodeCommandResponse*>(response);

		ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(ret == nullptr, L"Respose error.", __FILE__, __LINE__);


		UnicodeString* message = ret->toString(); __STP(message);
		logger->debugLog(ISystemLogger::DEBUG_NODE_TRANSFER_RESPONSE, message, __FILE__, __LINE__);

		__STP_MV(response);
	}

	return ret;
}

const AbstractBlockchainTransaction* OmittedBlockBodyFixer::get(const TransactionId *trxId) const noexcept {
	return this->map->get(trxId);
}

} /* namespace codablecash */
