/*
 * InstanceDriver.cpp
 *
 *  Created on: 2023/04/26
 *      Author: iizuka
 */

#include "InstanceDriver.h"

#include "base/StackRelease.h"

#include "base_io/File.h"

#include "bc/DebugDefaultLogger.h"
#include "bc/CodablecashNodeInstance.h"
#include "bc/exceptions.h"

#include "bc_trx_genesis/GenesisTransaction.h"

#include "bc_block/Block.h"

#include "bc_block_generator/MiningConfig.h"

#include "bc_status_cache/BlockchainController.h"
#include "blockchain/wallet_util/WalletDriver.h"
#include "osenv/funcs.h"

#include "bc_network/NodeIdentifier.h"
#include "bc_network/NodeIdentifierSource.h"

#include "bc_finalizer_trx/RegisterVotePoolTransaction.h"
#include "bc_finalizer_trx/RegisterTicketTransaction.h"

#include "bc_memorypool/MemoryPool.h"
#include "bc_memorypool/MemPoolTransaction.h"

#include "bc_base/AddressDescriptor.h"

#include "bc_status_cache_context/IStatusCacheContext.h"

#include "bc_trx/AbstractBlockchainTransaction.h"
#include "bc_trx/UtxoId.h"

#include "bc_p2p_cmd_client/SendTransactionClientCommand.h"

#include "bc_p2p_client/P2pClient.h"

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "bc_p2p_client/StackClientListnerRemover.h"


#include "TransactionWaiter.h"

namespace codablecash {

InstanceDriver::InstanceDriver(const File* baseDir) {
	this->instDir = baseDir->get(L"inst");
	this->walletDir = baseDir->get(L"wallet");

	this->walletDriver = nullptr;
	this->inst = nullptr;
	this->source = nullptr;

	this->logger = new DebugDefaultLogger();
}

InstanceDriver::~InstanceDriver() {
	if(this->inst != nullptr){
		this->inst->shutdown();
	}
	delete this->inst;

	delete this->walletDriver;


	delete this->instDir;
	delete this->walletDir;
	delete this->source;

	delete this->logger;
}

void InstanceDriver::initWallet(int numWallet) {
	this->walletDriver = new WalletDriver(0, this->walletDir, L"changeit");
	this->walletDriver->init(numWallet);
}

void InstanceDriver::initInstance(const CodablecashSystemParam *config) {
	{
		CodablecashNodeInstance inst(this->instDir, this->logger, config);
		bool bl = inst.initBlankInstance(0, 1);
		if(!bl){
			throw new MulformattedTransactionBinaryException(__FILE__, __LINE__);
		}
	}

	this->inst = new CodablecashNodeInstance(this->instDir, this->logger, config);
	this->inst->load();

	if(this->source == nullptr){
		this->source = NodeIdentifierSource::create();
	}
}

void InstanceDriver::initInstance(CodablecashSystemParam *config, const IInstanceMemberAllocator *alloc) {
	{
		CodablecashNodeInstance inst(this->instDir, this->logger, config, alloc);
		bool bl = inst.initBlankInstance(0, 1);
		if(!bl){
			throw new MulformattedTransactionBinaryException(__FILE__, __LINE__);
		}
	}

	this->inst = new CodablecashNodeInstance(this->instDir, this->logger, config, alloc);
	this->inst->load();
}

void InstanceDriver::startMiner(MiningConfig* mconfig) {
	AddressDescriptor* desc = this->walletDriver->getAddressDesc(0, 1); __STP(desc);
	mconfig->setAddressDescriptor(desc);

	this->inst->startBlockGenerator(mconfig);
}

void InstanceDriver::startFinalize() {
	// register pool
	if(this->source == nullptr){
		this->source = NodeIdentifierSource::create();
	}

	this->inst->startBlockFinalizer(this->source);
}

void InstanceDriver::initGenesisBalance(int index, uint64_t defaultBalance) {
	BlockchainController* ctrl = this->inst->getController();
	{
		GenesisTransaction* gtrx = this->walletDriver->createGenesisTransaction(index, defaultBalance); __STP(gtrx);
		this->walletDriver->importTransaction(gtrx);

		Block block(0, 1);
		block.setGenesisTimestamp();

		block.addBalanceTransaction(gtrx);
		block.build();

		ctrl->addBlock(&block);
	}
}

void InstanceDriver::registerVotingNode() {
	// register pool
	if(this->source == nullptr){
		this->source = NodeIdentifierSource::create();
	}
	NodeIdentifier nodeId = this->source->toNodeIdentifier();
	{
		RegisterVotePoolTransaction* trx = this->walletDriver->createRegisterVotePoolTransaction(0, this->source, BalanceUnit(1L)); __STP(trx);

		this->walletDriver->importTransaction(trx);

		MemoryPool* pool = this->inst->getMemoryPool();
		pool->putTransaction(trx);

		// debug
		{
			AbstractUtxo* utxo = trx->getUtxo(0);
			const UtxoId* utxoId = utxo->getId();

			TransactionId* trxId = pool->getTransactionIdFromUtxoId(utxoId); __STP(trxId);
			assert(trxId != nullptr);
			assert(trxId->equals(trx->getTransactionId()));

			UnicodeString* utxoIdstr = utxoId->toString(); __STP(utxoIdstr);
			this->logger->debugLog(ISystemLogger::DEBUG_UTXO_ID, utxoIdstr, __FILE__, __LINE__);
		}
	}
}

void InstanceDriver::registerVotingNode(P2pClient *client) {
	if(this->source == nullptr){
		this->source = NodeIdentifierSource::create();
	}

	NodeIdentifier nodeId = this->source->toNodeIdentifier();
	{
		RegisterVotePoolTransaction* trx = this->walletDriver->createRegisterVotePoolTransaction(0, this->source, BalanceUnit(1L)); __STP(trx);

		this->walletDriver->importTransaction(trx);

		const TransactionId* trxId = trx->getTransactionId();
		TransactionWaiter waiter(trxId);
		StackClientListnerRemover remover(client, &waiter);

		SendTransactionClientCommand cmd;
		cmd.setNodeIdentifier(&nodeId);
		cmd.setTransaction(trx);

		cmd.sign(client->getNodeIdentifierSource());

		AbstractCommandResponse* response = client->sendCommnad(&cmd); __STP(response);
		{
			UnicodeString* message = response->toString(); __STP(message);
			this->logger->debugLog(ISystemLogger::DEBUG_NODE_TRANSFER_RESPONSE, message, __FILE__, __LINE__);
		}

		waiter.wait();
	}
}

RegisterVotePoolTransaction* InstanceDriver::createRegisterVotePoolTransaction() {
	// register pool
	if(this->source == nullptr){
		this->source = NodeIdentifierSource::create();
	}
	NodeIdentifier nodeId = this->source->toNodeIdentifier();

	RegisterVotePoolTransaction* trx = this->walletDriver->createRegisterVotePoolTransaction(0, this->source, BalanceUnit(1L)); __STP(trx);
	return __STP_MV(trx);
}

void InstanceDriver::registerTicket(const BalanceUnit& stakeAmount) {
	if(this->source == nullptr){
		this->source = NodeIdentifierSource::create();
	}
	NodeIdentifier nodeId = this->source->toNodeIdentifier();
	{
		RegisterTicketTransaction* trx = this->walletDriver->createRegisterTicketTransaction(0, 0, 0, &nodeId, stakeAmount, BalanceUnit(1L)); __STP(trx);
		this->walletDriver->importTransaction(trx);

		const TransactionId* id = trx->getTransactionId();

		MemoryPool* pool = this->inst->getMemoryPool();

		AbstractControlTransaction* trxlast = pool->__getControlTransaction(id); __STP(trxlast);
		assert(trxlast == nullptr);

		pool->putTransaction(trx);
	}
}



void InstanceDriver::registerTicket(P2pClient *client, const BalanceUnit stakeAmount) {
	registerTicket(client, stakeAmount, nullptr);
}

void InstanceDriver::registerTicket(P2pClient *client, const BalanceUnit stakeAmount, MemoryPool *pool) {
	if(this->source == nullptr){
		this->source = NodeIdentifierSource::create();
	}
	NodeIdentifier nodeId = this->source->toNodeIdentifier();
	{
		RegisterTicketTransaction* trx = this->walletDriver->createRegisterTicketTransaction(0, 0, 0, &nodeId, stakeAmount, BalanceUnit(1L)); __STP(trx);
		this->walletDriver->importTransaction(trx);

		const TransactionId* trxId = trx->getTransactionId();
		TransactionWaiter waiter(trxId);
		StackClientListnerRemover remover(client, &waiter);

		SendTransactionClientCommand cmd;
		cmd.setTransaction(trx);

		cmd.sign(client->getNodeIdentifierSource());

		AbstractCommandResponse* response = client->sendCommnad(&cmd); __STP(response);
		{
			UnicodeString* message = response->toString(); __STP(message);
			this->logger->debugLog(ISystemLogger::DEBUG_NODE_TRANSFER_RESPONSE, message, __FILE__, __LINE__);
		}

		waiter.wait();


		if(pool != nullptr){
			bool exists = false;

			while(!exists){
				AbstractControlTransaction* regTrx = pool->getControlTransaction(trxId); __STP(regTrx);

				exists = (regTrx != nullptr);
				if(!exists){
					Os::usleep(100*1000);
				}
			}
		}
	}

}

RegisterTicketTransaction* InstanceDriver::createRegisterTicketTransaction(const BalanceUnit stakeAmount) {
	if(this->source == nullptr){
		this->source = NodeIdentifierSource::create();
	}
	NodeIdentifier nodeId = this->source->toNodeIdentifier();

	RegisterTicketTransaction* trx = this->walletDriver->createRegisterTicketTransaction(0, 0, 0, &nodeId, stakeAmount, BalanceUnit(1L)); __STP(trx);

	return __STP_MV(trx);
}

void InstanceDriver::waitUntilFinalizedHeight(uint64_t height) {
	waitUntilFinalizedHeight(height, this->inst);
}

void InstanceDriver::waitUntilFinalizedHeight(uint64_t height,	CodablecashNodeInstance *inst) {
	BlockchainController* ctrl = inst->getController();

	uint64_t h;
	do{
		Os::usleep(10000);
		h = ctrl->getFinalizedHeight(0);
	}
	while(h < height);
}

void InstanceDriver::waitUntilHeight(uint64_t height) {
	BlockchainController* ctrl = this->inst->getController();

	uint64_t h;
	do{
		Os::usleep(10000);
		h = ctrl->getHeadHeight(0);
	}
	while(h < height);
}

void InstanceDriver::sendBalance(BalanceUnit num, BalanceUnit feeRate) {
	BalanceTransferTransaction* trx = this->walletDriver->createBalanceTransferTransaction(0, 0, 1, num, feeRate, false); __STP(trx);

	this->walletDriver->importTransaction(trx);

	MemoryPool* pool = this->inst->getMemoryPool();
	{
		MemPoolTransaction* memTrx = pool->begin(); __STP(memTrx);
		BlockchainController* ctrl = this->inst->getController();
		IStatusCacheContext* context = ctrl->getStatusCacheContext(0); __STP(context);

		trx->validateOnAccept(memTrx, context);
	}

	pool->putTransaction(trx);
}

void InstanceDriver::sentTransaction(const AbstractBlockchainTransaction *trx) {
	// test copy
	AbstractBlockchainTransaction* trx2 = dynamic_cast<AbstractBlockchainTransaction*>(trx->copyData()); __STP(trx2);

	MemoryPool* pool = this->inst->getMemoryPool();
	{
		MemPoolTransaction* memTrx = pool->begin(); __STP(memTrx);
		BlockchainController* ctrl = this->inst->getController();
		IStatusCacheContext* context = ctrl->getStatusCacheContext(0); __STP(context);

		trx2->validateOnAccept(memTrx, context);
	}

	pool->putTransaction(trx2);
}


bool InstanceDriver::validateTransaction(const AbstractBlockchainTransaction *trx) const {
	MemoryPool* pool = this->inst->getMemoryPool();
	{
		MemPoolTransaction* memTrx = pool->begin(); __STP(memTrx);
		BlockchainController* ctrl = this->inst->getController();
		IStatusCacheContext* context = ctrl->getStatusCacheContext(0); __STP(context);

		return trx->validateOnAccept(memTrx, context);
	}
}

BalanceTransferTransaction* InstanceDriver::createBalanceTransferTransaction(BalanceUnit num, BalanceUnit feeRate) {
	BalanceTransferTransaction* trx = this->walletDriver->createBalanceTransferTransaction(0, 0, 1, num, feeRate, false); __STP(trx);

	return __STP_MV(trx);
}

void InstanceDriver::transferAbstractBalanceTransaction(const AbstractBalanceTransaction *trx) {
	MemoryPool* pool = this->inst->getMemoryPool();
	pool->putTransaction(trx);
}

void InstanceDriver::suspendMiner() {
	this->inst->suspendBlockGenerator();

	while(!this->inst->isBlockGeneratorSuspendStatus()){
		Os::usleep(100);
	}
}

void InstanceDriver::resumeMiner() {
	this->inst->resumeBlockGenerator();
}

void InstanceDriver::waitUntilVotingNodeRegistered() {
	NodeIdentifier nodeId = this->source->toNodeIdentifier();
	bool ret = false;

	while(!ret){
		BlockchainController* ctrl = this->inst->getController();
		uint16_t zone = ctrl->getZoneSelf();

		const VoterEntry* entry = nullptr;
		{
			IStatusCacheContext* context = ctrl->getStatusCacheContext(zone); __STP(context);
			entry = context->getVoterEntry(&nodeId);
		}

		if(entry != nullptr){
			ret = true;
		}
		else{
			Os::usleep(10*1000);
		}
	}
}

BlockchainController* InstanceDriver::getBlockchainController() const noexcept {
	return this->inst->getController();
}

void InstanceDriver::startV6Listner(int port) {
	this->inst->startNetwork(nullptr, port);
	this->inst->startProcessors(this->source, false);
}

} /* namespace codablecash */
