/*
 * SmartcontractTrxPool.cpp
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#include "bc_memorypool/SmartcontractTrxPool.h"

#include "bc_smartcontract/AbstractSmartcontractTransaction.h"

namespace codablecash {

SmartcontractTrxPool::SmartcontractTrxPool(const File* baseDir) : AbstractTrxMemoryPool(baseDir) {

}

SmartcontractTrxPool::~SmartcontractTrxPool() {

}

void SmartcontractTrxPool::putTransaction(const AbstractSmartcontractTransaction *trx) {
	AbstractTrxMemoryPool::putTransaction(trx);
}

void SmartcontractTrxPool::removeTransaction(const AbstractSmartcontractTransaction *trx) {
	AbstractTrxMemoryPool::removeTransaction(trx->getTransactionId());
}

AbstractSmartcontractTransaction* SmartcontractTrxPool::getSmartcontractTransaction(const TransactionId *trxId) const {
	return dynamic_cast<AbstractSmartcontractTransaction*>(AbstractTrxMemoryPool::getBlockchainTransaction(trxId));
}

} /* namespace codablecash */
