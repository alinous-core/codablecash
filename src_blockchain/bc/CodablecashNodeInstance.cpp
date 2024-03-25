/*
 * CodablecashNodeInstance.cpp
 *
 *  Created on: 2023/03/04
 *      Author: iizuka
 */

#include "bc/CodablecashNodeInstance.h"
#include "bc/DefaultInstanceMemberAllocator.h"
#include "bc/ISystemLogger.h"

#include "base/StackRelease.h"
#include "base/Exception.h"

#include "base_io/File.h"

#include "p2pserver/P2pServer.h"

#include "bc_blockstore/CodablecashBlockchain.h"

#include "bc_memorypool/MemoryPool.h"

#include "bc_status_cache/BlockchainStatusCache.h"
#include "bc_status_cache/BlockchainController.h"

#include "pow/PoWManager.h"

#include "bc_block_generator/BlockGenerator.h"

#include "bc/CodablecashConfig.h"

#include "bc_processor/CentralProcessor.h"

#include "bc_p2p_processor/P2pRequestProcessor.h"

#include "bc_p2p/BlochchainP2pManager.h"

#include "bc_finalizer_pool/FinalizerPool.h"

#include "bc_p2p_info/P2pDnsManager.h"

#include "pubsub/PubSubId.h"
#include "pubsub/P2pHandshake.h"

#include "bc_p2p_cmd/LoginPubSubCommand.h"
#include "bc_p2p_cmd/P2pHandshakeAuthenticationException.h"

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "bc_network/NodeIdentifierSource.h"
#include "bc_network/NodeIdentifier.h"

#include "bc_p2p_info/P2pNodeRecord.h"

#include "base/ArrayList.h"

#include "bc_p2p_selector/ConnectNodeSelector.h"
namespace codablecash {

CodablecashNodeInstance::CodablecashNodeInstance(const File* baseDir, ISystemLogger* logger, const CodablecashConfig* config) {
	DefaultInstanceMemberAllocator defalloc;
	this->allocator = defalloc.copy();

	__init(baseDir, logger, config);
}

CodablecashNodeInstance::CodablecashNodeInstance(const File *baseDir,	ISystemLogger *logger, const CodablecashConfig *config,
		const IInstanceMemberAllocator *allocator) {
	this->allocator = allocator->copy();

	__init(baseDir, logger, config);

}

void CodablecashNodeInstance::__init(const File *baseDir, ISystemLogger *logger, const CodablecashConfig *config) {
	this->config = new CodablecashConfig(*config);

	this->centralProcessor = nullptr;

	this->logger = logger;
	this->baseDir = new File(*baseDir);

	this->blockchain = nullptr;
	this->memoryPool = nullptr;
	this->statusCache = nullptr;
	this->ctrl = nullptr;

	this->powManager = nullptr;
	this->blockGenerator = nullptr;

	this->finalizer = nullptr;

	this->p2pServer = nullptr;
	this->p2pManager = nullptr;
	this->p2pRequestProcessor = nullptr;
	this->p2pDnsManager = nullptr;
	this->nodeName = nullptr;
}

CodablecashNodeInstance::~CodablecashNodeInstance() {
	shutdown();

	delete this->p2pServer;
	delete this->baseDir;
	delete this->config;

	delete this->allocator;
	delete this->nodeName;
}

bool CodablecashNodeInstance::initBlankInstance(uint16_t zoneSelf, uint16_t numZones) {
	try{
		File* blockChainDir = this->baseDir->get(BLOCKCHAIN_DIR); __STP(blockChainDir);
		File* memoryPoolBaseDir = this->baseDir->get(MEMORY_POOL_DIR); __STP(memoryPoolBaseDir);
		File* statusCacheBaseDir = this->baseDir->get(STATUS_CACHE_DIR); __STP(statusCacheBaseDir);
		File* tmpCacheBaseDir = this->baseDir->get(TMP_CACHE_DIR); __STP(tmpCacheBaseDir); // managed by ctrl

		CodablecashBlockchain::createBlankBlockchain(blockChainDir, zoneSelf, numZones);

		MemoryPool pool(memoryPoolBaseDir);
		pool.createBlankPool();

		BlockchainStatusCache cache(statusCacheBaseDir, this->config, &pool, tmpCacheBaseDir, this->logger);
		cache.initBlankCache(zoneSelf, numZones);

		P2pDnsManager p2pDatabase(this->baseDir);
		p2pDatabase.createBlankDatabase();

		P2pRequestProcessor requestProcessor(this->baseDir, this->p2pManager, this->config, this->logger);
		requestProcessor.createBlank(this);
	}
	catch(Exception* e){
		this->logger->logException(e);
		delete e;
		return false;
	}

	return true;
}

void CodablecashNodeInstance::load() {
	File* blockChainDir = this->baseDir->get(BLOCKCHAIN_DIR); __STP(blockChainDir);
	File* memoryPoolBaseDir = this->baseDir->get(MEMORY_POOL_DIR); __STP(memoryPoolBaseDir);
	File* statusCacheBaseDir = this->baseDir->get(STATUS_CACHE_DIR); __STP(statusCacheBaseDir);
	File* tmpCacheBaseDir = this->baseDir->get(TMP_CACHE_DIR); __STP(tmpCacheBaseDir);

	this->blockchain = new CodablecashBlockchain(blockChainDir);
	this->blockchain->open();

	this->memoryPool = new MemoryPool(memoryPoolBaseDir);
	this->memoryPool->open();

	this->statusCache = new BlockchainStatusCache(statusCacheBaseDir, this->config, this->memoryPool, tmpCacheBaseDir, this->logger);
	this->statusCache->open();

	this->ctrl = new BlockchainController(this->logger, this->config, tmpCacheBaseDir, this->blockchain, this->statusCache, this->memoryPool);

	this->centralProcessor = new CentralProcessor(this->config, this->memoryPool, this->ctrl, this->logger);
	this->centralProcessor->start();

	this->blockchain->setProcessor(this->centralProcessor);

	this->p2pDnsManager = new P2pDnsManager(this->baseDir);
	this->p2pDnsManager->open();
}

void CodablecashNodeInstance::initCacheStatus() {
	this->statusCache->initCacheStatus(this->blockchain);
}

void CodablecashNodeInstance::startNetwork(int port) {
	this->p2pServer = new P2pServer(this->logger, this);
	this->p2pManager = new BlochchainP2pManager();

	this->p2pManager->init(this->blockchain->getNumZones());

	this->p2pServer->addConnectionListener(this->p2pManager);

	this->p2pServer->startIpV6Listening(nullptr, port);

	this->centralProcessor->setBlochchainP2pManager(this->p2pManager);
}

void CodablecashNodeInstance::startProcessors(const NodeIdentifierSource *networkKey, bool suspend) {
	if(this->p2pRequestProcessor == nullptr){
		this->p2pRequestProcessor = new P2pRequestProcessor(this->baseDir, this->p2pManager, this->config, this->logger);
		this->p2pRequestProcessor->open(networkKey, this, suspend);

		this->centralProcessor->setP2pRequestProcessor(this->p2pRequestProcessor);
	}
}

void CodablecashNodeInstance::shutdownNetwork() {
	if(this->p2pServer != nullptr){
		this->p2pServer->stopListning();
		this->p2pManager->endCommunication();

		this->p2pManager->resetZones();
		this->p2pServer->dispose();

		delete this->p2pManager;
		this->p2pManager = nullptr;

		delete this->p2pServer;
		this->p2pServer = nullptr;
	}

	if(this->p2pRequestProcessor != nullptr){
		this->p2pRequestProcessor->close();
	}

	delete this->p2pRequestProcessor;
	this->p2pRequestProcessor = nullptr;

}

void CodablecashNodeInstance::startBlockGenerator(const MiningConfig *config) {
	uint16_t zone = this->blockchain->getZoneSelf();

	this->powManager = new PoWManager(this->logger);
	this->blockGenerator = this->allocator->newBlockGenerator(zone, this->config, this->memoryPool, this->ctrl, config, this->logger);
	this->blockGenerator->setCentralProcessor(this->centralProcessor);

	this->powManager->setBlockGenerator(this->blockGenerator);

	this->statusCache->setPowManager(this->powManager);

	this->powManager->start();
}

void CodablecashNodeInstance::startBlockFinalizer(const NodeIdentifierSource *nodeSource) {
	this->finalizer = this->allocator->newFinalizerPool(this->config, this->memoryPool, this->ctrl
			, this->p2pRequestProcessor, this->p2pManager, this->logger);
	this->finalizer->setVoterIdentifier(nodeSource);

	this->statusCache->setFinalizer(this->finalizer);

	this->finalizer->start();
}

void CodablecashNodeInstance::shutdown() {
	if(this->p2pServer != nullptr){ // stop network acception
		this->p2pServer->stopListning();
		this->p2pManager->endCommunication();
	}

	if(this->powManager != nullptr){
		this->ctrl->stopMining();
		this->powManager->shutdown();
	}

	if(this->finalizer != nullptr){
		this->ctrl->stopFinalizing();
		this->finalizer->shutdown();
	}

	if(this->centralProcessor != nullptr){
		this->centralProcessor->shutdown();
	}

	shutdownNetwork();

	if(this->blockchain != nullptr){
		this->blockchain->close();
		delete this->blockchain, this->blockchain = nullptr;
	}
	if(this->memoryPool != nullptr){
		this->memoryPool->close();
		delete this->memoryPool, this->memoryPool = nullptr;
	}
	if(this->statusCache != nullptr){
		this->statusCache->close();
		delete this->statusCache, this->statusCache = nullptr;
	}
	if(this->p2pDnsManager != nullptr){
		this->p2pDnsManager->close();
		delete this->p2pDnsManager;
		this->p2pDnsManager = nullptr;
	}

	delete this->powManager;
	this->powManager = nullptr;

	delete this->finalizer;
	this->finalizer = nullptr;

	delete this->blockGenerator;
	this->blockGenerator = nullptr;

	delete this->centralProcessor;
	this->centralProcessor = nullptr;

	delete this->ctrl;
	this->ctrl = nullptr;

}

void CodablecashNodeInstance::suspendBlockGenerator() {
	this->powManager->suspend();
}

void CodablecashNodeInstance::resumeBlockGenerator() {
	this->powManager->resume();
}

bool CodablecashNodeInstance::isBlockGeneratorSuspendStatus() const noexcept {
	return this->powManager->isSuspendStatus();
}

void CodablecashNodeInstance::connectIpV6Node(uint16_t zone, const UnicodeString *host, int port) {
	ERROR_POINT(L"CodablecashNodeInstance::connectIpV6Node");

	doConnectIpNode(zone, host, port, IP_V6);
}

void CodablecashNodeInstance::connectIpV4Node(uint16_t zone, const UnicodeString *host, int port) {
	ERROR_POINT(L"CodablecashNodeInstance::connectIpV4Node");

	doConnectIpNode(zone, host, port, IP_V4);
}

void CodablecashNodeInstance::doConnectIpNode(uint16_t zone, const UnicodeString* host, int port, int protocol){
	PubSubId* psId = PubSubId::createNewId(); __STP(psId);
	P2pHandshake *handshake = new P2pHandshake(psId, this->logger); __STP(handshake);
	handshake->init(this->p2pServer->getConnectionManager(), this);

	// connect
	if(protocol == IP_V4){
		handshake->connectIpV4(host, port, handshake, false);
	}else{
		handshake->connectIpV6(host, port, handshake, false);
	}

	// login
	loginNode(zone, handshake);

	// register connection
	this->p2pServer->registerHandshake(__STP_MV(handshake));

	// register node
	try{
		NodeIdentifierSource* source = this->p2pRequestProcessor->getNetworkKey();
		NodeIdentifier nodeId = source->toNodeIdentifier();
		this->p2pManager->registerHandshake(zone, handshake, &nodeId);
	}
	catch(Exception* e){
		handshake->dispose();
		throw e;
	}

	// start subscribe
	handshake->startSubscribe();
}

void CodablecashNodeInstance::loginNode(uint16_t zone, P2pHandshake *handshake) {
	// login
	NodeIdentifierSource* source = this->p2pRequestProcessor->getNetworkKey();

	LoginPubSubCommand cmd(zone);
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

NodeIdentifierSource* CodablecashNodeInstance::getNetworkKey() const noexcept  {
	return this->p2pRequestProcessor->getNetworkKey();
}

uint16_t CodablecashNodeInstance::getZoneSelf() const noexcept {
	return this->blockchain->getZoneSelf();
}

int CodablecashNodeInstance::getNumZones() const noexcept {
	return this->blockchain->getNumZones();
}

void CodablecashNodeInstance::maintainNetwork() {
	int numZones = getNumZones();

	for(int i = 0; i != numZones; ++i){
		__maintainNetwork(i);
	}
}

void CodablecashNodeInstance::__maintainNetwork(uint16_t zone) {
	int numNeedConnection = this->config->getNumConnectionExtShard();
	if(zone == getZoneSelf()){
		numNeedConnection = this->config->getNumConnectionOwnShard();
	}

	int currentSize = this->p2pManager->getNumNodes(zone);
	int diff = numNeedConnection - currentSize;

	if(diff > 0){
		ArrayList<P2pNodeRecord>* list = this->p2pDnsManager->getZoneRecords(zone, 1024); __STP(list);
		list->setDeleteOnExit();

		ConnectNodeSelector selector;
		selector.importCandidates(list);

		while(diff > 0 && selector.hasNext()){
			const P2pNodeRecord* rec = selector.fetchNext();

			if(__connectWithP2pNodeRecord(rec)){
				diff--;
			}
		}
	}
}

bool CodablecashNodeInstance::__connectWithP2pNodeRecord(const P2pNodeRecord *record) {
	try{
		int protocol = record->getProtocol();
		const UnicodeString* host = record->getHost();
		int port = record->getPort();
		uint16_t zone = record->getZone();

		if(protocol == P2pNodeRecord::TCP_IP_V4){
			connectIpV4Node(zone, host, port);
		}
		else if(protocol == P2pNodeRecord::TCP_IP_V6){
			connectIpV6Node(zone, host, port);
		}
	}
	catch(Exception* e){
		__STP(e);

		return false;
	}

	return true;
}

void CodablecashNodeInstance::resumeRequestProcessor() {
	this->p2pRequestProcessor->resume(this);
}

File* CodablecashNodeInstance::getTempCacheDir() const {
	return this->baseDir->get(TMP_CACHE_DIR);
}

void CodablecashNodeInstance::setNodeName(const wchar_t *name) noexcept {
	delete this->nodeName;
	this->nodeName = new UnicodeString(name);

	if(this->p2pRequestProcessor != nullptr){
		this->p2pRequestProcessor->setNodeName(this->nodeName);
	}
}

} /* namespace codablecash */
