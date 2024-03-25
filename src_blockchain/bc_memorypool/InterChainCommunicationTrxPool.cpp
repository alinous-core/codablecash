/*
 * InterChainCommunicationTrxPool.cpp
 *
 *  Created on: 2023/11/30
 *      Author: iizuka
 */

#include "bc_memorypool/InterChainCommunicationTrxPool.h"

#include "bc_trx/AbstractInterChainCommunicationTansaction.h"

namespace codablecash {

InterChainCommunicationTrxPool::InterChainCommunicationTrxPool(const File* baseDir) : AbstractTrxMemoryPool(baseDir) {

}

InterChainCommunicationTrxPool::~InterChainCommunicationTrxPool() {
;
}

void InterChainCommunicationTrxPool::putTransaction(const AbstractInterChainCommunicationTansaction *trx) {
	AbstractTrxMemoryPool::putTransaction(trx);
}

void InterChainCommunicationTrxPool::removeTransaction(const AbstractInterChainCommunicationTansaction *trx) {
	AbstractTrxMemoryPool::removeTransaction(trx->getTransactionId());
}

AbstractInterChainCommunicationTansaction* InterChainCommunicationTrxPool::getInterChainCommunicationTansaction(const TransactionId *trxId) const {
	return dynamic_cast<AbstractInterChainCommunicationTansaction*>(AbstractTrxMemoryPool::getBlockchainTransaction(trxId));
}

} /* namespace codablecash */
