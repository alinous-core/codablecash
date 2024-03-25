/*
 * MemPoolTransaction.h
 *
 *  Created on: 2023/02/28
 *      Author: iizuka
 */

#ifndef BC_MEMORYPOOL_MEMPOOLTRANSACTION_H_
#define BC_MEMORYPOOL_MEMPOOLTRANSACTION_H_

#include "base/ArrayList.h"

#include "bc_trx/TransactionId.h"

namespace alinous {
class ConcurrentGate;
class BtreeScanner;
}
using namespace alinous;

namespace codablecash {

class MemoryPool;
class TransactionId;
class FeeBananceTransactionScanner;
class ControlTransactionScanner;
class AbstractControlTransaction;
class BalanceTransferTransaction;
class BlockHeaderId;
class IStatusCacheContext;
class AbstractBlockchainTransaction;
class AbstractBalanceTransaction;
class BlockBody;
class BlockHeader;
class UtxoId;
class AbstractUtxo;
class MempoolUtxoFindResult;
class AbstractInterChainCommunicationTansaction;
class AbstractSmartcontractTransaction;
class InterChainCommunicationTransactionScanner;
class SmartcontractTansactionScanner;

class ITransactionListCallback;

class MemPoolTransaction {
public:
	explicit MemPoolTransaction(MemoryPool* pool, ConcurrentGate* rwLock);
	virtual ~MemPoolTransaction();

	MemPoolTransaction* newSubTransaction() const;

	void consume(const TransactionId* trxId);
	bool isConsumed(const TransactionId* trxId) const;

	FeeBananceTransactionScanner* beginScanBalanceTransaction();
	ControlTransactionScanner* beginScanControlTransaction();
	InterChainCommunicationTransactionScanner* beginScanInterChainCommunicationTransaction();
	SmartcontractTansactionScanner* beginScanSmartcontractTansaction();

	void removeControlTransaction(AbstractControlTransaction* trx);
	void removeBalanceTransaction(AbstractBalanceTransaction* trx);
	void removeInterChainCommunicationTransaction(AbstractInterChainCommunicationTansaction* trx);
	void removeSmartcontractTransaction(AbstractSmartcontractTransaction* trx);

	void cleanOnFinalize(IStatusCacheContext* context, uint64_t finalizingHeight, const BlockHeader *header, uint64_t lastFinalizedHeight);

	MemoryPool* getPool() const noexcept {
		return this->pool;
	}

	void putTransaction(const AbstractBlockchainTransaction* trx);

	AbstractControlTransaction* getControlTransaction(const TransactionId* trxId) const;
	bool hasControlTransaction(const TransactionId* trxId) const;

	AbstractBalanceTransaction* getBalanceTransaction(const TransactionId* trxId) const;
	bool hasBalanceTransaction(const TransactionId* trxId) const;

	AbstractInterChainCommunicationTansaction* getInterChainCommunicationTansaction(const TransactionId* trxId) const;
	bool hasInterChainCommunicationTansaction(const TransactionId* trxId) const;

	AbstractSmartcontractTransaction* getSmartcontractTransaction(const TransactionId* trxId) const;
	bool hasSmartcontractTransaction(const TransactionId* trxId) const;

	MempoolUtxoFindResult* findUtxo(const UtxoId *utxoId) const;

	int getTransactionsCount() const;

	void scanSyncBalanceTransaction(int offset, int limit, ITransactionListCallback* callback);
	void scanSyncControlTransaction(int offset, int limit, ITransactionListCallback* callback);
	void scanSyncInterChainCommunicationTransaction(int offset, int limit, ITransactionListCallback* callback);
	void scanSyncSmartcontractTransaction(int offset, int limit, ITransactionListCallback* callback);

	ArrayList<TransactionId>* countVotes(uint64_t targetHeight) const noexcept;

private:
	void scanSyncTransaction(BtreeScanner* scanner, int offset, int limit, ITransactionListCallback* callback);

	void removeInvalidTransactions(IStatusCacheContext* context, const BlockHeader *header);
	void removeImportedTransactions(IStatusCacheContext* context, uint64_t finalizingHeight, const BlockHeaderId *headerId, uint64_t lastFinalizedHeight);
	void removeTransactionsInBody(const BlockBody* body);

private:
	MemoryPool* pool;
	ConcurrentGate* rwLock;
	ArrayList<TransactionId, TransactionId::ValueCompare>* consumedList;
};

class ITransactionListCallback {
public:
	virtual ~ITransactionListCallback(){};
	virtual void addBlockchainTransaction(const AbstractBlockchainTransaction* trx) noexcept = 0;
};

} /* namespace codablecash */

#endif /* BC_MEMORYPOOL_MEMPOOLTRANSACTION_H_ */
