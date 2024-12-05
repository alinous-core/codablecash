/*
 * MemPoolSyncWorker.cpp
 *
 *  Created on: 2023/11/03
 *      Author: iizuka
 */

#include "bc_network_instance_sync/MemPoolSyncWorker.h"
#include "bc_network_instance_sync/NetworkNodeSync.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_network_instance/CodablecashNetworkNode.h"

#include "bc/CodablecashNodeInstance.h"
#include "bc_p2p_cmd_node/SyncMempoolNodeCommand.h"

#include "bc_p2p/BlochchainP2pManager.h"
#include "bc_p2p/BlockchainNodeHandshake.h"
#include "bc_p2p/StackHandshakeReleaser.h"
#include "bc_p2p/BlockchainNodeHandshakeException.h"

#include "base/StackRelease.h"
#include "bc/CodablecashSystemParam.h"

#include "bc/ExceptionThrower.h"

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "bc_p2p_cmd_node/SyncMempoolNodeCommandResponse.h"
#include "bc_p2p_cmd_node/InvalidTransactionException.h"

#include "bc_memorypool/MemoryPool.h"

#include "bc_trx/AbstractBlockchainTransaction.h"
#include "bc_trx/TransactionId.h"
#include "bc_trx/AbstractBalanceTransaction.h"
#include "bc_trx/AbstractControlTransaction.h"
#include "bc_trx/AbstractInterChainCommunicationTansaction.h"

#include "bc_smartcontract/AbstractSmartcontractTransaction.h"

#include "bc/ISystemLogger.h"
namespace codablecash {

MemPoolSyncWorker::MemPoolSyncWorker(const NodeIdentifier* nodeId, NetworkNodeSync* parent, ISystemLogger* logger, const UnicodeString* name)
		: AbstractSyncWorkerThread(logger, name) {
	this->nodeId = new NodeIdentifier(*nodeId);
	this->parent = parent;
}

MemPoolSyncWorker::~MemPoolSyncWorker() {
	delete this->nodeId;
	this->parent = nullptr;
}

void MemPoolSyncWorker::doProcess() {
	CodablecashNetworkNode* node = this->parent->getCodablecashNetworkNode();
	CodablecashNodeInstance* inst = node->getInstance();
	CodablecashSystemParam* param = inst->getCodablecashSystemParam();
	ISystemLogger* logger = inst->getLogger();

	const NodeIdentifierSource *source = inst->getNetworkKey();

	uint16_t zoneSelf = inst->getZoneSelf();

	BlochchainP2pManager* p2pManager = inst->getBlochchainP2pManager();
	MemoryPool* pool = inst->getMemoryPool();

	doSyncControlTransactions(p2pManager, pool, source, logger);
	doSyncInterChainCommunicationTransactions(p2pManager, pool, source, logger);
	doSyncBalanceTransactions(p2pManager, pool, source, logger);
	doSyncSmartcontractTransactions(p2pManager, pool, source, logger);
}

void MemPoolSyncWorker::doSyncBalanceTransactions(BlochchainP2pManager *p2pManager, MemoryPool* pool, const NodeIdentifierSource *source, ISystemLogger* logger) {
	int offset = 0;
	int limit = 100;
	bool hasnext = true;

	while(hasnext){
		SyncMempoolNodeCommand cmd;
		cmd.setTrxType(SyncMempoolNodeCommand::TRX_BALANCE);
		cmd.setOffset(offset);
		cmd.setNumRequestTrx(limit);

		cmd.sign(source);

		BlockchainNodeHandshake* handshake = p2pManager->getNodeHandshakeByNodeId(this->nodeId);
		ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(handshake == nullptr, L"Node connection has alrealy closed.", __FILE__, __LINE__);

		StackHandshakeReleaser __releaser(handshake);

		AbstractCommandResponse* response = handshake->sendCommnad(&cmd); __STP(response);
		{
			UnicodeString* str = response->toString(); __STP(str);
			logger->debugLog(ISystemLogger::DEBUG_NODE_TRANSFER_RESPONSE, str, __FILE__, __LINE__);
		}
		SyncMempoolNodeCommandResponse* syncResponse = dynamic_cast<SyncMempoolNodeCommandResponse*>(response);

		ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(syncResponse == nullptr, L"Respose error.", __FILE__, __LINE__);

		int nread = syncResponse->getResultCount();
		hasnext = (limit == nread);

		for(int i = 0; i != nread; ++i){
			const AbstractBlockchainTransaction* trx = syncResponse->get(i);
			const AbstractBalanceTransaction* btrx = dynamic_cast<const AbstractBalanceTransaction*>(trx);

			ExceptionThrower<InvalidTransactionException>::throwExceptionIfCondition(btrx == nullptr
					, L"Balance transaction is required.", __FILE__, __LINE__);

			const TransactionId* trxId = trx->getTransactionId();

			AbstractBalanceTransaction* lastTrx = pool->getBalanceTransaction(trxId); __STP(lastTrx);
			if(lastTrx == nullptr){
				pool->putTransaction(trx);
			}
		}
	}
}

void MemPoolSyncWorker::doSyncControlTransactions(BlochchainP2pManager *p2pManager, MemoryPool* pool, const NodeIdentifierSource *source, ISystemLogger* logger) {
	int offset = 0;
	int limit = 100;
	bool hasnext = true;

	while(hasnext){
		SyncMempoolNodeCommand cmd;
		cmd.setTrxType(SyncMempoolNodeCommand::TRX_CONTROL);
		cmd.setOffset(offset);
		cmd.setNumRequestTrx(limit);

		cmd.sign(source);

		BlockchainNodeHandshake* handshake = p2pManager->getNodeHandshakeByNodeId(this->nodeId);
		ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(handshake == nullptr, L"Node connection has alrealy closed.", __FILE__, __LINE__);

		StackHandshakeReleaser __releaser(handshake);

		AbstractCommandResponse* response = handshake->sendCommnad(&cmd); __STP(response);
		{
			UnicodeString* str = response->toString(); __STP(str);
			logger->debugLog(ISystemLogger::DEBUG_NODE_TRANSFER_RESPONSE, str, __FILE__, __LINE__);
		}
		SyncMempoolNodeCommandResponse* syncResponse = dynamic_cast<SyncMempoolNodeCommandResponse*>(response);

		ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(syncResponse == nullptr, L"Respose error.", __FILE__, __LINE__);

		int nread = syncResponse->getResultCount();
		hasnext = (limit == nread);

		for(int i = 0; i != nread; ++i){
			const AbstractBlockchainTransaction* trx = syncResponse->get(i);
			const AbstractControlTransaction* ctrx = dynamic_cast<const AbstractControlTransaction*>(trx);

			ExceptionThrower<InvalidTransactionException>::throwExceptionIfCondition(ctrx == nullptr
					, L"Control transaction is required.", __FILE__, __LINE__);

			const TransactionId* trxId = trx->getTransactionId();

			AbstractControlTransaction* lastTrx = pool->getControlTransaction(trxId); __STP(lastTrx);
			if(lastTrx == nullptr){
				pool->putTransaction(trx);
			}
		}
	}
}

void MemPoolSyncWorker::doSyncSmartcontractTransactions(BlochchainP2pManager *p2pManager, MemoryPool* pool, const NodeIdentifierSource *source, ISystemLogger* logger) {
	int offset = 0;
	int limit = 100;
	bool hasnext = true;

	while(hasnext){
		SyncMempoolNodeCommand cmd;
		cmd.setTrxType(SyncMempoolNodeCommand::TRX_SMARTCONTRACT);
		cmd.setOffset(offset);
		cmd.setNumRequestTrx(limit);

		cmd.sign(source);

		BlockchainNodeHandshake* handshake = p2pManager->getNodeHandshakeByNodeId(this->nodeId);
		ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(handshake == nullptr, L"Node connection has alrealy closed.", __FILE__, __LINE__);

		StackHandshakeReleaser __releaser(handshake);

		AbstractCommandResponse* response = handshake->sendCommnad(&cmd); __STP(response);
		{
			UnicodeString* str = response->toString(); __STP(str);
			logger->debugLog(ISystemLogger::DEBUG_NODE_TRANSFER_RESPONSE, str, __FILE__, __LINE__);
		}
		SyncMempoolNodeCommandResponse* syncResponse = dynamic_cast<SyncMempoolNodeCommandResponse*>(response);

		ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(syncResponse == nullptr, L"Respose error.", __FILE__, __LINE__);

		int nread = syncResponse->getResultCount();
		hasnext = (limit == nread);

		for(int i = 0; i != nread; ++i){
			const AbstractBlockchainTransaction* trx = syncResponse->get(i);
			const AbstractSmartcontractTransaction* sctrx = dynamic_cast<const AbstractSmartcontractTransaction*>(trx);

			ExceptionThrower<InvalidTransactionException>::throwExceptionIfCondition(sctrx == nullptr
					, L"Inter Chain Communication transaction is required.", __FILE__, __LINE__);

			const TransactionId* trxId = trx->getTransactionId();

			AbstractSmartcontractTransaction* lastTrx = pool->getSmartcontractTransaction(trxId); __STP(lastTrx);
			if(lastTrx == nullptr){
				pool->putTransaction(trx);
			}
		}
	}
}

void MemPoolSyncWorker::doSyncInterChainCommunicationTransactions(	BlochchainP2pManager *p2pManager, MemoryPool *pool, const NodeIdentifierSource *source, ISystemLogger* logger) {
	int offset = 0;
	int limit = 100;
	bool hasnext = true;

	while(hasnext){
		SyncMempoolNodeCommand cmd;
		cmd.setTrxType(SyncMempoolNodeCommand::TRX_ICC);
		cmd.setOffset(offset);
		cmd.setNumRequestTrx(limit);

		cmd.sign(source);

		BlockchainNodeHandshake* handshake = p2pManager->getNodeHandshakeByNodeId(this->nodeId);
		ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(handshake == nullptr, L"Node connection has alrealy closed.", __FILE__, __LINE__);

		StackHandshakeReleaser __releaser(handshake);

		AbstractCommandResponse* response = handshake->sendCommnad(&cmd); __STP(response);
		{
			UnicodeString* str = response->toString(); __STP(str);
			logger->debugLog(ISystemLogger::DEBUG_NODE_TRANSFER_RESPONSE, str, __FILE__, __LINE__);
		}
		SyncMempoolNodeCommandResponse* syncResponse = dynamic_cast<SyncMempoolNodeCommandResponse*>(response);

		ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(syncResponse == nullptr, L"Respose error.", __FILE__, __LINE__);

		int nread = syncResponse->getResultCount();
		hasnext = (limit == nread);

		for(int i = 0; i != nread; ++i){
			const AbstractBlockchainTransaction* trx = syncResponse->get(i);
			const AbstractInterChainCommunicationTansaction* icctrx = dynamic_cast<const AbstractInterChainCommunicationTansaction*>(trx);

			ExceptionThrower<InvalidTransactionException>::throwExceptionIfCondition(icctrx == nullptr
					, L"Inter Chain Communication transaction is required.", __FILE__, __LINE__);

			const TransactionId* trxId = trx->getTransactionId();

			AbstractInterChainCommunicationTansaction* lastTrx = pool->getInterChainCommunicationTansaction(trxId); __STP(lastTrx);
			if(lastTrx == nullptr){
				pool->putTransaction(trx);
			}
		}
	}
}

} /* namespace codablecash */
