/*
 * ControlTrxPool.cpp
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#include "bc_memorypool/ControlTrxPool.h"

#include "bc_trx/AbstractControlTransaction.h"

namespace codablecash {

ControlTrxPool::ControlTrxPool(const File* baseDir) : AbstractTrxMemoryPool(baseDir) {

}

ControlTrxPool::~ControlTrxPool() {

}

void ControlTrxPool::putTransaction(const AbstractControlTransaction *trx) {
	AbstractTrxMemoryPool::putTransaction(trx);
}

void ControlTrxPool::removeTransaction(const AbstractControlTransaction *trx) {
	AbstractTrxMemoryPool::removeTransaction(trx->getTransactionId());
}

AbstractControlTransaction* ControlTrxPool::getControlTransaction(	const TransactionId *trxId) const {
	return dynamic_cast<AbstractControlTransaction*>(AbstractTrxMemoryPool::getBlockchainTransaction(trxId));
}

} /* namespace codablecash */
