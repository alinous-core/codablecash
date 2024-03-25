/*
 * BalanceTransferTrxPool.h
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#ifndef BC_MEMORYPOOL_BALANCETRANSFERTRXPOOL_H_
#define BC_MEMORYPOOL_BALANCETRANSFERTRXPOOL_H_

#include "bc_memorypool/AbstractTrxMemoryPool.h"

namespace codablecash {

class TransactionId;
class AbstractBalanceTransaction;

class BalanceTransferTrxPool : public AbstractTrxMemoryPool {
public:
	explicit BalanceTransferTrxPool(const File* baseDir);
	virtual ~BalanceTransferTrxPool();

	void putTransaction(const AbstractBalanceTransaction* trx);
	void removeTransaction(const AbstractBalanceTransaction* trx);

	AbstractBalanceTransaction* getBalanceTransaction(const TransactionId* trxId) const;
};

} /* namespace codablecash */

#endif /* BC_MEMORYPOOL_BALANCETRANSFERTRXPOOL_H_ */
