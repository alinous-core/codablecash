/*
 * NodeConnectionSimulator.cpp
 *
 *  Created on: 2023/12/04
 *      Author: iizuka
 */

#include "NodeConnectionSimulator.h"

#include "pubsub/P2pHandshake.h"
#include "pubsub/PubSubId.h"

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "p2pserver/P2pServerConnectionManager.h"

#include "bc_network/NodeIdentifierSource.h"

#include "bc_p2p_cmd/LoginPubSubCommand.h"
#include "bc_p2p_cmd/P2pHandshakeAuthenticationException.h"

#include "bc_p2p_cmd_node/DownloadTransactionEntry.h"

#include "base/StackRelease.h"

#include "bc/ISystemLogger.h"

#include "bc_p2p_cmd_node/AbstractNodeCommand.h"

#include "base_io/File.h"

#include "bc_memorypool/MemoryPool.h"

#include "bc_processor_cmd/TransferedMinedReportCommandMessage.h"

#include "bc_block/BlockHeaderId.h"

#include "bc_p2p_cmd_node/DownloadTransactionsNodeCommand.h"

#include "bc_p2p_cmd_node/DownloadTransactionsNodeCommandResponse.h"

#include "bc_trx/AbstractBlockchainTransaction.h"

#include "bc_trx/TransactionId.h"
namespace codablecash {

NodeConnectionSimulator::NodeConnectionSimulator(ISystemLogger* logger, const File* baseDir) {
	this->logger = logger;
	this->psId = nullptr;
	this->handshake = nullptr;
	this->source = NodeIdentifierSource::create();

	this->connectionManager = new P2pServerConnectionManager(this->logger, this);

	File* mempoolBaseDir = baseDir->get(L"mempool"); __STP(mempoolBaseDir);
	this->memPool = new MemoryPool(mempoolBaseDir);
	this->memPool->createBlankPool();
	this->memPool->open();

	this->entrylist = new ArrayList<DownloadTransactionEntry>();
	this->height = 0;
	this->headerId = nullptr;

	this->map = new HashMap<TransactionId, AbstractBlockchainTransaction>();
}

NodeConnectionSimulator::~NodeConnectionSimulator() {
	close();
	this->logger = nullptr;
	delete this->source;

	this->entrylist->deleteElements();
	delete this->entrylist;
	delete this->headerId;

	Iterator<TransactionId>* it = this->map->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const TransactionId* trxId = it->next();
		AbstractBlockchainTransaction* trx = this->map->get(trxId);

		delete trx;
	}

	delete this->map;
}

void NodeConnectionSimulator::fireExecuteCommand(const PubSubId* pubsubId, const AbstractPubSubCommand *cmd) {
}

void NodeConnectionSimulator::fireOnExeptionOnSubscriber(const PubSubId* pubsubId, const Exception *e) {
}

void NodeConnectionSimulator::fireOnSubscribeEnds(const PubSubId* pubsubId) {
}

IPubsubCommandExecutor* NodeConnectionSimulator::getExecutor() const noexcept {
	return nullptr;
}

void NodeConnectionSimulator::connectIpv6Node(uint16_t zone, const UnicodeString *host, int port) {
	this->psId = PubSubId::createNewId();
	this->handshake = new P2pHandshake(this->psId, this->logger);
	this->handshake->init(this->connectionManager, this);

	this->handshake->connectIpV6(host, port, this, false);
	this->handshake->startSubscribe();

	// login
	loginNode(zone, this->handshake);
}

void NodeConnectionSimulator::loginNode(uint16_t zone, P2pHandshake *handshake) {
	LoginPubSubCommand cmd(zone, nullptr);
	cmd.sign(source);

	AbstractCommandResponse* response = handshake->publishCommand(&cmd); __STP(response);
	if(response != nullptr){
		UnicodeString* msg = response->toString(); __STP(msg);
		this->logger->debugLog(ISystemLogger::DEBUG_NODE_TRANSFER_RESPONSE, msg, __FILE__, __LINE__);
	}

	uint32_t type = response->getType();
	if(type != AbstractCommandResponse::TYPE_RES_OK){
		throw new P2pHandshakeAuthenticationException(L"Already connected.", __FILE__, __LINE__);
	}
}

void NodeConnectionSimulator::close() {
	if(this->handshake != nullptr){
		delete this->psId;
		this->psId = nullptr;

		this->handshake->dispose();
		delete this->handshake;
		this->handshake = nullptr;
	}

	if(this->connectionManager != nullptr){
		this->connectionManager->dispose();
		delete this->connectionManager;
		this->connectionManager = nullptr;
	}

	if(this->memPool != nullptr){
		this->memPool->close();
		delete this->memPool;
		this->memPool = nullptr;
	}
}

AbstractCommandResponse* NodeConnectionSimulator::signAndAbstractNodeCommand(const AbstractNodeCommand *commnad) {
	AbstractNodeCommand* cmd = dynamic_cast<AbstractNodeCommand*>(commnad->copyData()); __STP(cmd);

	cmd->sign(this->source);

	AbstractCommandResponse* res = this->handshake->publishCommand(cmd);
	{
		UnicodeString* str = res->toString(); __STP(str);
		this->logger->debugLog(ISystemLogger::DEBUG_NODE_TRANSFER_RESPONSE, str, __FILE__, __LINE__);
	}

	return res;
}

MemPoolTransaction* NodeConnectionSimulator::begin() {
	return this->memPool->begin();
}


NodeIdentifierSource* NodeConnectionSimulator::getNetworkKey() const noexcept {
	return this->source;
}


void NodeConnectionSimulator::addDownloadTransactionEntry(const DownloadTransactionEntry *entry) {
	this->entrylist->addElement(dynamic_cast<DownloadTransactionEntry*>(entry->copyData()));
}

void NodeConnectionSimulator::downloadTransactions(ISystemLogger *logger) {
	DownloadTransactionsNodeCommand command;
	command.setHeight(this->height);
	command.setBlockHeaderId(this->headerId);

	{
		int maxLoop = this->entrylist->size();
		for(int i = 0; i != maxLoop; ++i){
			DownloadTransactionEntry* entry = this->entrylist->get(i);
			command.addTrxId(entry);
		}
	}

	{
		AbstractCommandResponse* rs = signAndAbstractNodeCommand(&command); __STP(rs);
		DownloadTransactionsNodeCommandResponse* response = dynamic_cast<DownloadTransactionsNodeCommandResponse*>(rs);

		int maxLoop = response->size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractBlockchainTransaction* trx = response->get(i);
			const TransactionId* trxId = trx->getTransactionId();

			this->map->put(trxId, dynamic_cast<AbstractBlockchainTransaction*>(trx->copyData()));
		}
	}
}

const AbstractBlockchainTransaction* NodeConnectionSimulator::get(const TransactionId *trxId) const noexcept {
	return this->map->get(trxId);
}

void NodeConnectionSimulator::setCommandData(uint64_t height, const BlockHeaderId *headerId) {
	delete this->headerId;

	this->height = height;
	this->headerId = dynamic_cast<BlockHeaderId*>(headerId->copyData());

	// reset map
	Iterator<TransactionId>* it = this->map->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const TransactionId* trxId = it->next();
		AbstractBlockchainTransaction* trx = this->map->get(trxId);

		delete trx;
	}
	this->map->clear();

	// reset entry list
	this->entrylist->deleteElements();
	this->entrylist->reset();
}

} /* namespace codablecash */
