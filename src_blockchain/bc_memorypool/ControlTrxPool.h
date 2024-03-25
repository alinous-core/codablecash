/*
 * ControlTrxPool.h
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#ifndef BC_MEMORYPOOL_CONTROLTRXPOOL_H_
#define BC_MEMORYPOOL_CONTROLTRXPOOL_H_

#include "bc_memorypool/AbstractTrxMemoryPool.h"

namespace alinous {
class File;
}
using namespace alinous;

namespace codablecash {

class AbstractControlTransaction;
class TransactionId;
class UtxoId;
class AbstractUtxo;
class MempoolUtxoFindResult;

class ControlTrxPool : public AbstractTrxMemoryPool {
public:
	explicit ControlTrxPool(const File* baseDir);
	virtual ~ControlTrxPool();

	void putTransaction(const AbstractControlTransaction* trx);
	void removeTransaction(const AbstractControlTransaction* trx);

	AbstractControlTransaction* getControlTransaction(const TransactionId* trxId) const;
};

} /* namespace codablecash */

#endif /* BC_MEMORYPOOL_CONTROLTRXPOOL_H_ */
