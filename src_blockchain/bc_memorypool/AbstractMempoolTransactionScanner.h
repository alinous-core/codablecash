/*
 * AbstractMempoolTransactionScanner.h
 *
 *  Created on: 2023/12/02
 *      Author: iizuka
 */

#ifndef BC_MEMORYPOOL_ABSTRACTMEMPOOLTRANSACTIONSCANNER_H_
#define BC_MEMORYPOOL_ABSTRACTMEMPOOLTRANSACTIONSCANNER_H_

namespace alinous {
class BtreeReverseScanner;
}
using namespace alinous;

namespace codablecash {

class MemPoolTransaction;
class AbstractBlockchainTransaction;
class TransactionIdsListData;
class TransactionId;
class AbstractTrxMemoryPool;

class AbstractMempoolTransactionScanner {
public:
	AbstractMempoolTransactionScanner(MemPoolTransaction* memTrx, BtreeReverseScanner* feeTrxScanner, AbstractTrxMemoryPool* trxPool);
	virtual ~AbstractMempoolTransactionScanner();

	void init();
	bool hasNext();

protected:
	AbstractBlockchainTransaction* next() const;

private:
	bool isConsumed(const TransactionId* trxId) const noexcept;

protected:
	MemPoolTransaction* memTrx;
	AbstractTrxMemoryPool* trxPool;
	BtreeReverseScanner* feeTrxScanner;
	TransactionIdsListData* listData;
	const TransactionId* trxId;
};

} /* namespace codablecash */

#endif /* BC_MEMORYPOOL_ABSTRACTMEMPOOLTRANSACTIONSCANNER_H_ */
