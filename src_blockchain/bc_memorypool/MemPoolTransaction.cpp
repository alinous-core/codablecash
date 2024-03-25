/*
 * MemPoolTransaction.cpp
 *
 *  Created on: 2023/02/28
 *      Author: iizuka
 */

#include "bc_memorypool/MemPoolTransaction.h"
#include "bc_memorypool/FeeBananceTransactionScanner.h"
#include "bc_memorypool/MemoryPool.h"
#include "bc_memorypool/ControlTransactionScanner.h"
#include "bc_memorypool/MempoolUtxoFindResult.h"

#include "bc_trx/TransactionId.h"
#include "bc_trx/AbstractControlTransaction.h"
#include "bc_trx/AbstractInterChainCommunicationTansaction.h"
#include "bc_trx/AbstractBlockchainTransaction.h"

#include "bc_trx_balance/BalanceTransferTransaction.h"

#include "bc_status_cache_context/IStatusCacheContext.h"

#include "bc_blockstore/CodablecashBlockchain.h"

#include "bc_block/BlockHeaderId.h"
#include "bc_block/BlockHeader.h"

#include "bc_block_body/BlockBody.h"

#include "base/StackRelease.h"

#include "base_thread/ConcurrentGate.h"

#include "bc_blockstore_header/BlockHeaderStoreManager.h"

#include "bc_blockstore_body/BlockBodyStoreManager.h"

#include "btree/BtreeScanner.h"

#include "bc_base_trx_index/TransactionData.h"

#include "bc_smartcontract/AbstractSmartcontractTransaction.h"

#include "bc_memorypool/InterChainCommunicationTransactionScanner.h"

#include "bc_memorypool/SmartcontractTansactionScanner.h"
namespace codablecash {

MemPoolTransaction::MemPoolTransaction(MemoryPool* pool, ConcurrentGate* rwLock) {
	this->pool = pool;
	this->rwLock = rwLock;
	this->consumedList = new ArrayList<TransactionId, TransactionId::ValueCompare>();
}

MemPoolTransaction::~MemPoolTransaction() {
	this->pool = nullptr;

	if(this->rwLock != nullptr){
		this->rwLock->open();
	}

	this->consumedList->deleteElements();
	delete this->consumedList;
}

MemPoolTransaction* MemPoolTransaction::newSubTransaction() const {
	return new MemPoolTransaction(this->pool, nullptr);
}

void MemPoolTransaction::consume(const TransactionId* trxId) {
	TransactionId* id = dynamic_cast<TransactionId*>(trxId->copyData());
	this->consumedList->addElement(id);
}

bool MemPoolTransaction::isConsumed(const TransactionId *trxId) const {
	this->consumedList->sort();

	TransactionId* id = this->consumedList->search(trxId);
	return id != nullptr;
}

FeeBananceTransactionScanner* MemPoolTransaction::beginScanBalanceTransaction() {
	this->consumedList->sort();

	BtreeReverseScanner* feeTrxScanner = this->pool->getBalanceTransactionScanner();

	FeeBananceTransactionScanner* scanner = new FeeBananceTransactionScanner(this, feeTrxScanner);
	scanner->init();

	return scanner;
}

ControlTransactionScanner* MemPoolTransaction::beginScanControlTransaction() {
	this->consumedList->sort();

	BtreeReverseScanner* feeTrxScanner = this->pool->getControlTransactionScanner();

	ControlTransactionScanner* scanner = new ControlTransactionScanner(this, feeTrxScanner); __STP(scanner);
	scanner->init();

	return __STP_MV(scanner);
}

InterChainCommunicationTransactionScanner* MemPoolTransaction::beginScanInterChainCommunicationTransaction() {
	this->consumedList->sort();

	BtreeReverseScanner* feeTrxScanner = this->pool->getInterChainCommunicationTransactionScanner();

	InterChainCommunicationTransactionScanner* scanner = new InterChainCommunicationTransactionScanner(this, feeTrxScanner); __STP(scanner);
	scanner->init();

	return __STP_MV(scanner);
}

SmartcontractTansactionScanner* MemPoolTransaction::beginScanSmartcontractTansaction() {
	this->consumedList->sort();

	BtreeReverseScanner* feeTrxScanner = this->pool->getSmartcontractTransactionScanner();

	SmartcontractTansactionScanner* scanner = new SmartcontractTansactionScanner(this, feeTrxScanner); __STP(scanner);
	scanner->init();

	return __STP_MV(scanner);
}

void MemPoolTransaction::removeControlTransaction(AbstractControlTransaction *trx) {
	this->pool->__removeControlTransaction(trx);
}

void MemPoolTransaction::removeBalanceTransaction(AbstractBalanceTransaction *trx) {
	this->pool->__removeBalanceTransaction(trx);
}
void MemPoolTransaction::removeInterChainCommunicationTransaction(AbstractInterChainCommunicationTansaction *trx) {
	this->pool->__removeInterChainCommunicationTransaction(trx);
}

void MemPoolTransaction::removeSmartcontractTransaction(AbstractSmartcontractTransaction *trx) {
	this->pool->__removeSmartcontractTransaction(trx);
}

void MemPoolTransaction::putTransaction(const AbstractBlockchainTransaction *trx) {
	this->pool->__putTransaction(trx);
}

AbstractControlTransaction* MemPoolTransaction::getControlTransaction(const TransactionId *trxId) const {
	return this->pool->__getControlTransaction(trxId);
}

bool MemPoolTransaction::hasControlTransaction(const TransactionId *trxId) const {
	AbstractControlTransaction* trx = getControlTransaction(trxId); __STP(trx);
	return trx != nullptr;
}

AbstractBalanceTransaction* MemPoolTransaction::getBalanceTransaction(const TransactionId *trxId) const {
	return this->pool->__getBalanceTransaction(trxId);
}

bool MemPoolTransaction::hasBalanceTransaction(const TransactionId *trxId) const {
	AbstractBalanceTransaction* trx = getBalanceTransaction(trxId); __STP(trx);
	return trx != nullptr;
}

AbstractInterChainCommunicationTansaction* MemPoolTransaction::getInterChainCommunicationTansaction(const TransactionId *trxId) const {
	return this->pool->__getInterChainCommunicationTansaction(trxId);
}

bool MemPoolTransaction::hasInterChainCommunicationTansaction(const TransactionId *trxId) const {
	AbstractInterChainCommunicationTansaction* trx = getInterChainCommunicationTansaction(trxId); __STP(trx);
	return trx != nullptr;
}

AbstractSmartcontractTransaction* MemPoolTransaction::getSmartcontractTransaction(const TransactionId *trxId) const {
	return this->pool->__getSmartcontractTransaction(trxId);
}

bool MemPoolTransaction::hasSmartcontractTransaction(const TransactionId *trxId) const {
	AbstractSmartcontractTransaction* trx = getSmartcontractTransaction(trxId); __STP(trx);
	return trx != nullptr;
}

void MemPoolTransaction::cleanOnFinalize(IStatusCacheContext* context, uint64_t finalizingHeight
		, const BlockHeader *header, uint64_t lastFinalizedHeight) {

	const BlockHeaderId* headerId = header->getId();
	removeImportedTransactions(context, finalizingHeight, headerId, lastFinalizedHeight);
	removeInvalidTransactions(context, header);
}

void MemPoolTransaction::removeInvalidTransactions(IStatusCacheContext *context, const BlockHeader *header) {
	{
		ArrayList<AbstractControlTransaction> list;
		list.setDeleteOnExit();

		{
			ControlTransactionScanner* scanner = beginScanControlTransaction(); __STP(scanner);
			while(scanner->hasNext()){
				AbstractControlTransaction* trx = scanner->next(); __STP(trx);

				TrxValidationResult result = trx->validateFinal(header, this, context);
				if(result == TrxValidationResult::INVALID){
					list.addElement(__STP_MV(trx));
				}
			}
		}

		int maxLoop = list.size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractControlTransaction* trx = list.get(i);
			removeControlTransaction(trx);
		}
	}
	{
		ArrayList<AbstractInterChainCommunicationTansaction> list;
		list.setDeleteOnExit();

		{
			InterChainCommunicationTransactionScanner* scanner = beginScanInterChainCommunicationTransaction(); __STP(scanner);
			while(scanner->hasNext()){
				AbstractInterChainCommunicationTansaction* trx = scanner->next(); __STP(trx);

				TrxValidationResult result = trx->validateFinal(header, this, context);
				if(result == TrxValidationResult::INVALID){
					list.addElement(__STP_MV(trx));
				}
			}
		}

		int maxLoop = list.size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractInterChainCommunicationTansaction* trx = list.get(i);
			removeInterChainCommunicationTransaction(trx);
		}
	}
	{
		ArrayList<AbstractBalanceTransaction> list;
		list.setDeleteOnExit();

		{
			FeeBananceTransactionScanner* scanner = beginScanBalanceTransaction(); __STP(scanner);
			while(scanner->hasNext()){
				AbstractBalanceTransaction* trx = scanner->next(); __STP(trx);

				TrxValidationResult result = trx->validateFinal(header, this, context);
				if(result == TrxValidationResult::INVALID){
					list.addElement(__STP_MV(trx));
				}
			}
		}

		int maxLoop = list.size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractBalanceTransaction* trx = list.get(i);
			removeBalanceTransaction(trx);
		}
	}
	{
		ArrayList<AbstractSmartcontractTransaction> list;
		list.setDeleteOnExit();

		{
			SmartcontractTansactionScanner* scanner = beginScanSmartcontractTansaction(); __STP(scanner);
			while(scanner->hasNext()){
				AbstractSmartcontractTransaction* trx = scanner->next(); __STP(trx);

				TrxValidationResult result = trx->validateFinal(header, this, context);
				if(result == TrxValidationResult::INVALID){
					list.addElement(__STP_MV(trx));
				}
			}
		}

		int maxLoop = list.size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractSmartcontractTransaction* trx = list.get(i);
			removeSmartcontractTransaction(trx);
		}
	}
}

void MemPoolTransaction::removeImportedTransactions(	IStatusCacheContext *context, uint64_t finalizingHeight,
		const BlockHeaderId *headerId, uint64_t lastFinalizedHeight) {
	CodablecashBlockchain* chain = context->getBlockChain();
	uint16_t zone = context->getZone();

	BlockHeaderStoreManager* headerManager = chain->getHeaderManager(zone);
	BlockBodyStoreManager* bodyManager = chain->getBlockBodyStoreManager(zone);

	BlockHeaderId* currentHeaderId = dynamic_cast<BlockHeaderId*>(headerId->copyData());
	uint64_t height = finalizingHeight;

	while(height > lastFinalizedHeight){
		__STP(currentHeaderId);
		BlockHeader* header = headerManager->getHeader(currentHeaderId, height); __STP(header);

		const BlockMerkleRoot* root = header->getMerkleRoot();
		BlockBody* body = bodyManager->getBlockBody(root, height); __STP(body);
		removeTransactionsInBody(body);

		// for next loop
		height--;
		currentHeaderId = dynamic_cast<BlockHeaderId*>(header->getLastHeaderId()->copyData());
	}
	__STP(currentHeaderId);
}

void MemPoolTransaction::removeTransactionsInBody(const BlockBody *body) {
	{
		const ArrayList<AbstractControlTransaction>* list = body->getControlTransactions();

		int maxLoop = list->size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractControlTransaction* trx = list->get(i);

			removeControlTransaction(trx);
		}
	}
	{
		const ArrayList<AbstractInterChainCommunicationTansaction>* list = body->getInterChainCommunicationTransactions();

		int maxLoop = list->size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractInterChainCommunicationTansaction* trx = list->get(i);

			removeInterChainCommunicationTransaction(trx);
		}
	}
	{
		const ArrayList<AbstractBalanceTransaction>* list = body->getBalanceTransactions();

		int maxLoop = list->size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractBalanceTransaction* trx = list->get(i);

			removeBalanceTransaction(trx);
		}
	}
	{
		const ArrayList<AbstractSmartcontractTransaction>* list = body->getSmartcontractTransactions();

		int maxLoop = list->size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractSmartcontractTransaction* trx = list->get(i);

			removeSmartcontractTransaction(trx);
		}
	}
}

MempoolUtxoFindResult* MemPoolTransaction::findUtxo(const UtxoId *utxoId) const {
	MempoolUtxoFindResult* result = this->pool->__findUtxo(utxoId); __STP(result);
	MempoolUtxoFindResult* ret = nullptr;

	const TransactionId* trxId = result != nullptr ? result->getTransactionId() : nullptr;
	if(result != nullptr && trxId != nullptr && !isConsumed(trxId)){
		ret = __STP_MV(result);
	}

	return ret;
}

int MemPoolTransaction::getTransactionsCount() const {
	return this->pool->getTrxCount();
}

void MemPoolTransaction::scanSyncBalanceTransaction(int offset, int limit, ITransactionListCallback *callback) {
	BtreeScanner* sacnner = this->pool->__getBalanceSyncTransactionScanner(); __STP(sacnner);

	scanSyncTransaction(sacnner, offset, limit, callback);
}

void MemPoolTransaction::scanSyncControlTransaction(int offset, int limit, ITransactionListCallback *callback) {
	BtreeScanner* sacnner = this->pool->__getControlSyncTransactionScanner(); __STP(sacnner);

	scanSyncTransaction(sacnner, offset, limit, callback);
}

void MemPoolTransaction::scanSyncInterChainCommunicationTransaction(int offset,	int limit, ITransactionListCallback *callback) {
	BtreeScanner* sacnner = this->pool->__getInterChainCommunicationTransactionScanner(); __STP(sacnner);

	scanSyncTransaction(sacnner, offset, limit, callback);
}

void MemPoolTransaction::scanSyncSmartcontractTransaction(int offset, int limit, ITransactionListCallback *callback) {
	BtreeScanner* sacnner = this->pool->__getSmartcontractTransactionScanner(); __STP(sacnner);

	scanSyncTransaction(sacnner, offset, limit, callback);
}

void MemPoolTransaction::scanSyncTransaction(BtreeScanner *scanner, int offset, int limit, ITransactionListCallback *callback) {
	scanner->begin();

	int pos = 0;
	int count = 0;
	while(scanner->hasNext()){
		const IBlockObject* obj = scanner->next();

		if(offset > pos++){
			continue;
		}

		const TransactionData* data = dynamic_cast<const TransactionData*>(obj);
		const AbstractBlockchainTransaction* trx = data->getTrx();
		callback->addBlockchainTransaction(trx);
		count++;

		if(count >= limit){
			break;
		}
	}
}

ArrayList<TransactionId>* MemPoolTransaction::countVotes(uint64_t targetHeight) const noexcept {
	return this->pool->__countVotes(targetHeight);
}

} /* namespace codablecash */
