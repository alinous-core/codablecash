/*
 * ControlTransactionScanner.cpp
 *
 *  Created on: 2023/04/19
 *      Author: iizuka
 */

#include "bc_memorypool/ControlTransactionScanner.h"
#include "bc_memorypool/MemPoolTransaction.h"
#include "bc_memorypool/ControlTrxPool.h"
#include "bc_memorypool/MemoryPool.h"

#include "bc_base_trx_index/TransactionIdsListData.h"

#include "bc_trx/AbstractControlTransaction.h"

namespace codablecash {

ControlTransactionScanner::ControlTransactionScanner(MemPoolTransaction* memTrx, BtreeReverseScanner* feeTrxScanner)
		: AbstractMempoolTransactionScanner(memTrx, feeTrxScanner, memTrx->getPool()->getControlTrxPool()){

}

ControlTransactionScanner::~ControlTransactionScanner() {

}

AbstractControlTransaction* ControlTransactionScanner::next() const {
	return dynamic_cast<AbstractControlTransaction*>(AbstractMempoolTransactionScanner::next());
}

} /* namespace codablecash */
