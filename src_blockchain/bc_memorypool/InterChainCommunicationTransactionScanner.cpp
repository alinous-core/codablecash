/*
 * InterChainCommunicationTransactionScanner.cpp
 *
 *  Created on: 2023/12/02
 *      Author: iizuka
 */

#include "bc_memorypool/InterChainCommunicationTransactionScanner.h"
#include "bc_memorypool/MemPoolTransaction.h"
#include "bc_memorypool/MemoryPool.h"
#include "bc_memorypool/InterChainCommunicationTrxPool.h"

#include "bc_trx/AbstractInterChainCommunicationTansaction.h"

namespace codablecash {

InterChainCommunicationTransactionScanner::InterChainCommunicationTransactionScanner(MemPoolTransaction* memTrx, BtreeReverseScanner* feeTrxScanner)
		: AbstractMempoolTransactionScanner(memTrx, feeTrxScanner, memTrx->getPool()->getInterChainCommunicationTrxPool()){

}

InterChainCommunicationTransactionScanner::~InterChainCommunicationTransactionScanner() {

}

AbstractInterChainCommunicationTansaction* InterChainCommunicationTransactionScanner::next() const {
	return dynamic_cast<AbstractInterChainCommunicationTansaction*>(AbstractMempoolTransactionScanner::next());
}

} /* namespace codablecash */
