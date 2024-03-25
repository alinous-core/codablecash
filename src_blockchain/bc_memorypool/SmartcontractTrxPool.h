/*
 * SmartcontractTrxPool.h
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#ifndef BC_MEMORYPOOL_SMARTCONTRACTTRXPOOL_H_
#define BC_MEMORYPOOL_SMARTCONTRACTTRXPOOL_H_

#include "bc_memorypool/AbstractTrxMemoryPool.h"

namespace codablecash {

class AbstractSmartcontractTransaction;
class TransactionId;

class SmartcontractTrxPool : public AbstractTrxMemoryPool {
public:
	explicit SmartcontractTrxPool(const File* baseDir);
	virtual ~SmartcontractTrxPool();

	void putTransaction(const AbstractSmartcontractTransaction* trx);
	void removeTransaction(const AbstractSmartcontractTransaction* trx);

	AbstractSmartcontractTransaction* getSmartcontractTransaction(const TransactionId *trxId) const;
};

} /* namespace codablecash */

#endif /* BC_MEMORYPOOL_SMARTCONTRACTTRXPOOL_H_ */
