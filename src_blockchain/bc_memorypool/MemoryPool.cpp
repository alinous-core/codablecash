/*
 * MemoryPool.cpp
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#include "bc_memorypool/MemoryPool.h"
#include "bc_memorypool/BalanceTransferTrxPool.h"
#include "bc_memorypool/ControlTrxPool.h"
#include "bc_memorypool/SmartcontractTrxPool.h"
#include "bc_memorypool/MemoryPoolException.h"
#include "bc_memorypool/MemPoolTransaction.h"
#include "bc_memorypool/MempoolUtxoFindResult.h"
#include "bc_memorypool/InterChainCommunicationTrxPool.h"

#include "base/StackRelease.h"

#include "base_io/File.h"

#include "bc_trx_balance/BalanceTransferTransaction.h"

#include "bc_trx/AbstractControlTransaction.h"
#include "bc_trx/AbstractInterChainCommunicationTansaction.h"

#include "base_thread/ConcurrentGate.h"

#include "btree/BtreeReverseScanner.h"

#include "bc_finalizer_trx/VoteBlockTransaction.h"

#include "bc_base_trx_index/TransactionIdsListData.h"

#include "bc_smartcontract/AbstractSmartcontractTransaction.h"

namespace codablecash {

MemoryPool::MemoryPool(const File* memoryPoolBaseDir) {
	this->baseDir = new File(*memoryPoolBaseDir);

	File* controlDir = this->baseDir->get(L"control"); __STP(controlDir);
	this->controlPool = new ControlTrxPool(controlDir);

	File* iccDir = this->baseDir->get(L"icc"); __STP(iccDir);
	this->iccPool = new InterChainCommunicationTrxPool(iccDir);

	File* balanceDir = this->baseDir->get(L"balance"); __STP(balanceDir);
	this->balancePool = new BalanceTransferTrxPool(balanceDir);

	File* smartcontractDir = this->baseDir->get(L"smartcontract"); __STP(smartcontractDir);
	this->smartcontractPool = new SmartcontractTrxPool(smartcontractDir);

	this->rwLock = new ConcurrentGate();
}

MemoryPool::~MemoryPool() {
	delete this->baseDir;

	delete this->smartcontractPool;
	delete this->balancePool;
	delete this->iccPool;
	delete this->controlPool;

	delete this->rwLock;
}

bool MemoryPool::exists() const {
	bool balance = this->balancePool->exists();
	bool ctrl = this->controlPool->exists();


	return balance && ctrl;
}

void MemoryPool::createBlankPool() {
	StackWriteLock __lock(this->rwLock);

	this->controlPool->createBlankPool();
	this->iccPool->createBlankPool();
	this->balancePool->createBlankPool();
	this->smartcontractPool->createBlankPool();
}

void MemoryPool::open() {
	StackWriteLock __lock(this->rwLock);
	this->controlPool->open();
	this->iccPool->open();
	this->balancePool->open();
	this->smartcontractPool->open();
}

void MemoryPool::close() noexcept {
	StackWriteLock __lock(this->rwLock);

	this->balancePool->close();
	this->controlPool->close();
	this->iccPool->close();
	this->smartcontractPool->close();
}

void MemoryPool::putTransaction(const AbstractBlockchainTransaction *trx) {
	StackWriteLock __lock(this->rwLock);

	__putTransaction(trx);
}

void MemoryPool::__putTransaction(const AbstractBlockchainTransaction *trx) {
	const AbstractBalanceTransaction* btrx = dynamic_cast<const AbstractBalanceTransaction*>(trx);
	if(btrx != nullptr){
		this->balancePool->putTransaction(btrx);
		return;
	}
	const AbstractControlTransaction* ctrlTrx = dynamic_cast<const AbstractControlTransaction*>(trx);
	if(ctrlTrx != nullptr && ctrlTrx->canAddMempool() == true){
		this->controlPool->putTransaction(ctrlTrx);
		return;
	}
	const AbstractInterChainCommunicationTansaction* iccTrx = dynamic_cast<const AbstractInterChainCommunicationTansaction*>(trx);
	if(iccTrx != nullptr){
		this->iccPool->putTransaction(iccTrx);
		return;
	}
	const AbstractSmartcontractTransaction* scTrx = dynamic_cast<const AbstractSmartcontractTransaction*>(trx);
	if(scTrx != nullptr){
		this->smartcontractPool->putTransaction(scTrx);
		return;
	}

	throw new MemoryPoolException(L"Unsupported transaction type.", __FILE__, __LINE__);
}

void MemoryPool::removeTransaction(const TransactionId *trxId) {
	StackWriteLock __lock(this->rwLock);

	AbstractBalanceTransaction* balanceTrx = this->balancePool->getBalanceTransaction(trxId); __STP(balanceTrx);
	if(balanceTrx != nullptr){
		this->balancePool->removeTransaction(balanceTrx);
	}
}

void MemoryPool::__removeBalanceTransaction(const AbstractBalanceTransaction *trx) {
	this->balancePool->removeTransaction(trx);
}

void MemoryPool::__removeControlTransaction(const AbstractControlTransaction *trx) {
	this->controlPool->removeTransaction(trx);
}

void MemoryPool::__removeInterChainCommunicationTransaction(const AbstractInterChainCommunicationTansaction *trx) {
	this->iccPool->removeTransaction(trx);
}

void MemoryPool::__removeSmartcontractTransaction(const AbstractSmartcontractTransaction *trx) {
	this->smartcontractPool->removeTransaction(trx);
}

AbstractBlockchainTransaction* MemoryPool::getTransactionById(const TransactionId *trxId) const {
	StackReadLock __lock(this->rwLock);

	AbstractBlockchainTransaction* trx = __getBalanceTransaction(trxId);
	if(trx == nullptr){
		trx = __getControlTransaction(trxId);
	}
	if(trx == nullptr){
		trx = __getInterChainCommunicationTansaction(trxId);
	}
	if(trx == nullptr){
		trx = __getSmartcontractTransaction(trxId);
	}

	return trx;
}

AbstractBalanceTransaction* MemoryPool::getBalanceTransaction(const TransactionId *trxId) const {
	StackReadLock __lock(this->rwLock);

	return __getBalanceTransaction(trxId);
}

AbstractBalanceTransaction* MemoryPool::__getBalanceTransaction(const TransactionId *trxId) const {
	return this->balancePool->getBalanceTransaction(trxId);
}

AbstractControlTransaction* MemoryPool::getControlTransaction(const TransactionId *trxId) const {
	StackReadLock __lock(this->rwLock);

	return __getControlTransaction(trxId);
}

AbstractControlTransaction* MemoryPool::__getControlTransaction(const TransactionId *trxId) const {
	return this->controlPool->getControlTransaction(trxId);
}

AbstractInterChainCommunicationTansaction* MemoryPool::getInterChainCommunicationTansaction(const TransactionId *trxId) const {
	StackReadLock __lock(this->rwLock);

	return __getInterChainCommunicationTansaction(trxId);
}

AbstractInterChainCommunicationTansaction* MemoryPool::__getInterChainCommunicationTansaction(const TransactionId *trxId) const {
	return this->iccPool->getInterChainCommunicationTansaction(trxId);
}

AbstractSmartcontractTransaction* MemoryPool::getSmartcontractTransaction(const TransactionId *trxId) const {
	StackReadLock __lock(this->rwLock);

	return __getSmartcontractTransaction(trxId);
}

AbstractSmartcontractTransaction* MemoryPool::__getSmartcontractTransaction(const TransactionId *trxId) const {
	return this->smartcontractPool->getSmartcontractTransaction(trxId);
}

TransactionId* MemoryPool::getTransactionIdFromUtxoId(const UtxoId *utxoId) const {
	StackReadLock __lock(this->rwLock);

	return this->balancePool->getTransactionIdFromUtxoId(utxoId);
}

ArrayList<TransactionId>* MemoryPool::countVotes(uint64_t targetHeight) const noexcept {
	StackReadLock __lock(this->rwLock);

	return __countVotes(targetHeight);
}

ArrayList<TransactionId>* MemoryPool::__countVotes(uint64_t targetHeight) const noexcept {
	ArrayList<TransactionId>* result = new ArrayList<TransactionId>(); __STP(result);
	result->setDeleteOnExit();

	BtreeReverseScanner* scanner = this->controlPool->getScanner(); __STP(scanner);
	scanner->begin();

	while(scanner->hasPrevious()){
		const IBlockObject* obj = scanner->previous();
		const TransactionIdsListData* data = dynamic_cast<const TransactionIdsListData*>(obj);

		assert(data != nullptr);

		// transaction data
		const ArrayList<TransactionId>* list = data->getList();
		int maxLoop = list->size();
		for(int i = 0; i != maxLoop; ++i){
			TransactionId* id = list->get(i);

			AbstractControlTransaction* abtrx = __getControlTransaction(id); __STP(abtrx);
			const VoteBlockTransaction* trx = dynamic_cast<const VoteBlockTransaction*>(abtrx);
			if(trx != nullptr && trx->getVotedHeaderBlockHeight() == targetHeight){
				result->addElement(dynamic_cast<TransactionId*>(id->copyData()));
			}
		}
	}

	return __STP_MV(result);
}

MemPoolTransaction* MemoryPool::begin() noexcept {
	this->rwLock->close();
	MemPoolTransaction* mtrx = new MemPoolTransaction(this, this->rwLock);
	return mtrx;
}

BtreeReverseScanner* MemoryPool::getBalanceTransactionScanner() const noexcept {
	return this->balancePool->getScanner();
}

BtreeReverseScanner* MemoryPool::getControlTransactionScanner() const noexcept {
	return this->controlPool->getScanner();
}

BtreeReverseScanner* MemoryPool::getInterChainCommunicationTransactionScanner() const noexcept {
	return this->iccPool->getScanner();
}

BtreeReverseScanner* MemoryPool::getSmartcontractTransactionScanner() const noexcept {
	return this->smartcontractPool->getScanner();
}

MempoolUtxoFindResult* MemoryPool::__findUtxo(const UtxoId *utxoId) const {
	MempoolUtxoFindResult* utxo = this->balancePool->getUtxo(utxoId);
	if(utxo == nullptr){
		utxo = this->controlPool->getUtxo(utxoId);
	}

	return utxo;
}

BtreeScanner* MemoryPool::__getBalanceSyncTransactionScanner() const noexcept {
	return this->balancePool->getSyncScanner();
}

BtreeScanner* MemoryPool::__getControlSyncTransactionScanner() const noexcept {
	return this->controlPool->getSyncScanner();
}

BtreeScanner* MemoryPool::__getInterChainCommunicationTransactionScanner() const noexcept {
	return this->iccPool->getSyncScanner();
}

BtreeScanner* MemoryPool::__getSmartcontractTransactionScanner() const noexcept {
	return this->smartcontractPool->getSyncScanner();
}

int MemoryPool::getTrxCount() const {
	int total = this->balancePool->getCount();
	total += this->controlPool->getCount();
	total += this->smartcontractPool->getCount();
	total += this->iccPool->getCount();

	return total;
}

} /* namespace codablecash */
