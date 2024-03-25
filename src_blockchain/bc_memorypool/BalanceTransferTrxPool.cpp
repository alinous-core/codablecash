/*
 * BalanceTransferTrxPool.cpp
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#include "bc_memorypool/BalanceTransferTrxPool.h"

#include "bc_trx/AbstractBalanceTransaction.h"

namespace codablecash {

BalanceTransferTrxPool::BalanceTransferTrxPool(const File* baseDir) : AbstractTrxMemoryPool(baseDir) {

}

BalanceTransferTrxPool::~BalanceTransferTrxPool() {
}

void BalanceTransferTrxPool::putTransaction(const AbstractBalanceTransaction *trx) {
	AbstractTrxMemoryPool::putTransaction(trx);
}

void BalanceTransferTrxPool::removeTransaction(const AbstractBalanceTransaction *trx) {
	AbstractTrxMemoryPool::removeTransaction(trx->getTransactionId());
}

AbstractBalanceTransaction* BalanceTransferTrxPool::getBalanceTransaction(const TransactionId *trxId) const {
	return dynamic_cast<AbstractBalanceTransaction*>(AbstractTrxMemoryPool::getBlockchainTransaction(trxId));
}

} /* namespace codablecash */
