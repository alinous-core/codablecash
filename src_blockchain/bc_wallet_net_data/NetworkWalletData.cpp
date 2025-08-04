/*
 * NetworkCommandData.cpp
 *
 *  Created on: Mar 23, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_data/NetworkWalletData.h"
#include "bc_wallet_net_data/WalletMemoryPool.h"
#include "bc_wallet_net_data/HeaderTransactionGroup.h"
#include "bc_wallet_net_data/TransactionGroupDataStore.h"

#include "bc_wallet_net_data_management/ManagementAccountsCollection.h"
#include "bc_wallet_net_data_management/ManagementAccount.h"

#include "bc_wallet/HdWallet.h"
#include "bc_wallet/WalletAccount.h"

#include "base/StackRelease.h"

#include "base_io/File.h"

#include "bc_blockstore_header/BlockHeaderStoreManager.h"

#include "bc_blockstore/CodablecashBlockchain.h"

#include "bc_status_cache/HeadBlockDetector.h"
#include "bc_status_cache/BlockHeadElement.h"

#include "bc_block/BlockHeader.h"
#include "bc_block/BlockHeaderId.h"

#include "bc/CodablecashSystemParam.h"

#include "bc_base_conf_store/StatusStore.h"

#include "bc_trx/AbstractBlockchainTransaction.h"

#include "btree/BtreeScanner.h"

#include "bc_base_trx_index/TransactionData.h"

#include "base_thread/ConcurrentGate.h"

using alinous::StackWriteLock;

namespace codablecash {

NetworkWalletData::NetworkWalletData(const File* baseDir, ISystemLogger* logger, const CodablecashSystemParam *config) {
	this->baseDir = new File(*baseDir);
	this->logger = logger;
	this->config = config;

	this->hdWallet = nullptr;
	this->headerManager = nullptr;
	this->detector = nullptr;

	this->transactionGroupData = new TransactionGroupDataStore(this->baseDir);

	File* mempoolBaseDir = this->baseDir->get(L"mempool"); __STP(mempoolBaseDir);
	this->mempool = new WalletMemoryPool(mempoolBaseDir);

	this->finalizedHeight = 0;
	this->zone = 0;
	this->statusStore = nullptr;

	this->managementAccounts = new ManagementAccountsCollection();

	this->gateLock = new ConcurrentGate();
}

NetworkWalletData::~NetworkWalletData() {
	close();

	delete this->hdWallet;

	delete this->mempool;

	delete this->headerManager;
	delete this->detector;

	delete this->baseDir;
	this->logger = nullptr;

	delete this->managementAccounts;

	delete this->gateLock;
}

void NetworkWalletData::close() noexcept {
	StackWriteLock __lock(this->gateLock, __FILE__, __LINE__);

	if(this->transactionGroupData != nullptr){
		this->transactionGroupData->close();
		delete this->transactionGroupData;
		this->transactionGroupData = nullptr;
	}

	if(this->mempool != nullptr){
		this->mempool->close();
		delete this->mempool;
		this->mempool = nullptr;
	}

	if(this->statusStore != nullptr){
		this->statusStore->close();
		delete this->statusStore;
		this->statusStore = nullptr;
	}
}

void NetworkWalletData::createHdWallet(const HdWalletSeed *seed, uint16_t defaultZone, const IWalletDataEncoder *encoder, int defaultMaxAddress) {
	StackWriteLock __lock(this->gateLock, __FILE__, __LINE__);

	delete this->hdWallet;

	File* hdbase = this->baseDir->get(L"hd"); __STP(hdbase);

	this->hdWallet = HdWallet::create(hdbase, seed, defaultZone, encoder, defaultMaxAddress);

	this->zone = defaultZone;

	__initStatusStore();
	__saveStatus();
}

void NetworkWalletData::createBlank() {
	StackWriteLock __lock(this->gateLock, __FILE__, __LINE__);

	this->headerManager = new BlockHeaderStoreManager(this->baseDir, CodablecashBlockchain::DEFAULT_SECTION_LIMIT);
	this->detector = new HeadBlockDetector(this->logger);

	this->transactionGroupData->initBlank();
	this->transactionGroupData->open();

	this->mempool->createBlankPool();
	this->mempool->open();

	__initStatusStore();
	__saveStatus();
}

void NetworkWalletData::addTransactionDataToMempool(const TransactionTransferData *data) {
	StackWriteLock __lock(this->gateLock, __FILE__, __LINE__);

	// TODO addTransactionDataToMempool
}

uint16_t NetworkWalletData::getDefaultZone() const noexcept {
	StackReadLock __lock(this->gateLock, __FILE__, __LINE__);

	return this->hdWallet->getDefaultZone();
}

void NetworkWalletData::addHeader(const BlockHeader *header, ArrayList<AbstractBlockchainTransaction>* trxlist) {
	StackWriteLock __lock(this->gateLock, __FILE__, __LINE__);

	this->headerManager->addHeader(header);

	// trx group
	{
		HeaderTransactionGroup group;
		const BlockHeaderId* id = header->getId();
		group.setHeaderId(id);

		int maxLoop = trxlist->size();
		for(int i = 0; i != maxLoop; ++i){
			const AbstractBlockchainTransaction* trx = trxlist->get(i);

			group.addTransaction(trx);
		}

		this->transactionGroupData->add(id, &group);
	}

	// detect header
	this->detector->reset();
	// Make header lines
	this->detector->buildHeads(this->zone, this, this->finalizedHeight);

	// evaluate
	this->detector->evaluate(this->zone, nullptr, this, this->config, true);
	this->detector->selectChain();

	// check finalizing
	int votePerBlock = this->config->getVotePerBlock();
	if(header->isFinalizing(votePerBlock)){
		__doFinalize(header);
	}
}

void NetworkWalletData::__doFinalize(const BlockHeader *header) {
	// FIXME do finalize
}

void NetworkWalletData::__saveStatus() {
	this->statusStore->addShortValue(KEY_ZONE, this->zone);
	this->statusStore->addLongValue(KEY_FINALIZED_HEIGHT, this->finalizedHeight);
}

BlockHeaderStoreManager* NetworkWalletData::getHeaderManager(uint16_t zone) const noexcept {
	return this->headerManager;
}

BlockBodyStoreManager* NetworkWalletData::getBlockBodyStoreManager(uint16_t zone) const noexcept {
	return nullptr;
}

void NetworkWalletData::__initStatusStore() {
	if(this->statusStore == nullptr){
		this->statusStore = new StatusStore(this->baseDir, STATUS_STORE_FILE_NAME);
	}
}

void NetworkWalletData::resetManagementAccounts() noexcept {
	StackWriteLock __lock(this->gateLock, __FILE__, __LINE__);

	this->managementAccounts->resetAll();
}

void NetworkWalletData::buildManagementAccount(bool buildFinalized) {
	StackWriteLock __lock(this->gateLock, __FILE__, __LINE__);

	if(buildFinalized){
		__buildFinalizedManagementAccount();
	}

	// unfinalized
	__buildUnfinalizedAccount();

	// mempool
	__buildMempoolAccount();
}

void NetworkWalletData::__buildMempoolAccount() {
	WalletAccount* waccount = this->hdWallet->getZoneAccount(this->zone);

	ManagementAccount* uma = this->managementAccounts->getUnFinalizedManagementAccount();
	ManagementAccount* ma = this->managementAccounts->getMempoolManagementAccount();

	ma->reset();
	ma->importOtherAccount(uma);

	{
		BtreeScanner* scanner = this->mempool->getScanner(); __STP(scanner);
		scanner->begin();

		while(scanner->hasNext()){
			const IBlockObject* obj = scanner->next();
			const TransactionData* data = dynamic_cast<const TransactionData*>(obj);

			const AbstractBlockchainTransaction* trx = data->getTrx();
			ma->addTransaction(trx, waccount);
		}
	}
}

void NetworkWalletData::__buildUnfinalizedAccount() {
	ManagementAccount* fma = this->managementAccounts->getFinalizedManagementAccount();
	ManagementAccount* ma = this->managementAccounts->getUnFinalizedManagementAccount();

	ma->reset();
	ma->importOtherAccount(fma);

	// from detector
	{
		const BlockHead* head = this->detector->getHead();
		const ArrayList<BlockHeadElement>* list = head->getHeaders();

		int maxLoop = list->size();
		for(int i = 0; i != maxLoop; ++i){
			const BlockHeadElement* element = list->get(i);
			const BlockHeader* header = element->getBlockHeader();

			__buildManagementAccount4Header(ma, header);
		}
	}
}

void NetworkWalletData::__buildFinalizedManagementAccount() {
	ManagementAccount *ma = this->managementAccounts->getFinalizedManagementAccount();
	uint64_t startHeight = 1;
	uint64_t maxBlockHeight = this->finalizedHeight;

	__buildManagementAccount(ma, startHeight, maxBlockHeight);
}

void NetworkWalletData::__buildManagementAccount(ManagementAccount *ma, uint64_t startHeight, uint64_t maxBlockHeight) {
	uint64_t maxLoop = maxBlockHeight + 1;
	for(uint64_t i = startHeight; i != maxLoop; ++i){
		ArrayList<BlockHeader>* headerList = this->headerManager->getBlocksAtHeight(i); __STP(headerList);
		if(headerList != nullptr){
			headerList->setDeleteOnExit();

			int maxHeaders = headerList->size();
			for(int j = 0; i != maxHeaders; ++j){
				BlockHeader* header = headerList->get(j);

				__buildManagementAccount4Header(ma, header);
			}
		}
	}
}

void NetworkWalletData::__buildManagementAccount4Header(ManagementAccount *ma, const BlockHeader *header) {
	WalletAccount* waccount = this->hdWallet->getZoneAccount(this->zone);

	uint64_t height = header->getHeight();
	const BlockHeaderId* headerId = header->getId();

	HeaderTransactionGroup* trxGroup = this->transactionGroupData->getHeaderTransactionGroup(headerId); __STP(trxGroup);

	const ArrayList<AbstractBlockchainTransaction>* list = trxGroup->getTransactionsList();
	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		const AbstractBlockchainTransaction* trx = list->get(i);

		// check trx contains address
		if(trx->checkFilteredUxtoRef(ma) || trx->checkFilteredAddress(waccount)){
			ma->addTransaction(trx, waccount);
		}
	}
}

} /* namespace codablecash */
