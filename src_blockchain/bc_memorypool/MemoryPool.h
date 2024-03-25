/*
 * MemoryPool.h
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#ifndef BC_MEMORYPOOL_MEMORYPOOL_H_
#define BC_MEMORYPOOL_MEMORYPOOL_H_

#include <cstdint>

#include "base/ArrayList.h"

namespace alinous {
class File;
class ConcurrentGate;
class BtreeReverseScanner;
class BtreeScanner;
}
using namespace alinous;

namespace codablecash {

class BalanceTransferTrxPool;
class ControlTrxPool;
class SmartcontractTrxPool;
class AbstractBlockchainTransaction;
class BalanceTransferTransaction;
class TransactionId;
class UtxoId;
class MemPoolTransaction;
class AbstractBalanceTransaction;
class AbstractControlTransaction;
class AbstractUtxo;
class MempoolUtxoFindResult;
class InterChainCommunicationTrxPool;
class AbstractInterChainCommunicationTansaction;
class AbstractSmartcontractTransaction;

class MemoryPool {
public:
	explicit MemoryPool(const File* memoryPoolBaseDir);
	virtual ~MemoryPool();

	bool exists() const;

	void createBlankPool();

	void open();
	void close() noexcept;

	void putTransaction(const AbstractBlockchainTransaction* trx);
	void __putTransaction(const AbstractBlockchainTransaction* trx);
	void removeTransaction(const TransactionId* trxId);

	void __removeBalanceTransaction(const AbstractBalanceTransaction *trx);
	void __removeControlTransaction(const AbstractControlTransaction *trx);
	void __removeInterChainCommunicationTransaction(const AbstractInterChainCommunicationTansaction* trx);
	void __removeSmartcontractTransaction(const AbstractSmartcontractTransaction* trx);

	AbstractBlockchainTransaction* getTransactionById(const TransactionId* trxId) const;

	AbstractBalanceTransaction* getBalanceTransaction(const TransactionId* trxId) const;
	AbstractBalanceTransaction* __getBalanceTransaction(const TransactionId* trxId) const;
	TransactionId* getTransactionIdFromUtxoId(const UtxoId* utxoId) const;

	AbstractControlTransaction* getControlTransaction(const TransactionId* trxId) const;
	AbstractControlTransaction* __getControlTransaction(const TransactionId* trxId) const;

	AbstractInterChainCommunicationTansaction* getInterChainCommunicationTansaction(const TransactionId* trxId) const;
	AbstractInterChainCommunicationTansaction* __getInterChainCommunicationTansaction(const TransactionId* trxId) const;

	AbstractSmartcontractTransaction* getSmartcontractTransaction(const TransactionId* trxId) const;
	AbstractSmartcontractTransaction* __getSmartcontractTransaction(const TransactionId* trxId) const;

	ArrayList<TransactionId>* countVotes(uint64_t targetHeight) const noexcept;
	ArrayList<TransactionId>* __countVotes(uint64_t targetHeight) const noexcept;

	MempoolUtxoFindResult* __findUtxo(const UtxoId *utxoId) const;

	MemPoolTransaction* begin() noexcept;

	BtreeReverseScanner* getBalanceTransactionScanner() const noexcept;
	BtreeReverseScanner* getControlTransactionScanner() const noexcept;
	BtreeReverseScanner* getInterChainCommunicationTransactionScanner() const noexcept;
	BtreeReverseScanner* getSmartcontractTransactionScanner() const noexcept;

	BtreeScanner* __getBalanceSyncTransactionScanner() const noexcept;
	BtreeScanner* __getControlSyncTransactionScanner() const noexcept;
	BtreeScanner* __getInterChainCommunicationTransactionScanner() const noexcept;
	BtreeScanner* __getSmartcontractTransactionScanner() const noexcept;

	BalanceTransferTrxPool* getBalanceTransferTrxPool() const noexcept {
		return this->balancePool;
	}
	ControlTrxPool* getControlTrxPool() const noexcept {
		return this->controlPool;
	}
	InterChainCommunicationTrxPool* getInterChainCommunicationTrxPool() const noexcept {
		return this->iccPool;
	}
	SmartcontractTrxPool* getSmartcontractTrxPool() const noexcept {
		return this->smartcontractPool;
	}

	int getTrxCount() const;

private:
	File* baseDir;

	ControlTrxPool* controlPool;
	InterChainCommunicationTrxPool* iccPool;
	BalanceTransferTrxPool* balancePool;
	SmartcontractTrxPool* smartcontractPool;

	ConcurrentGate* rwLock;
};

} /* namespace codablecash */

#endif /* BC_MEMORYPOOL_MEMORYPOOL_H_ */
