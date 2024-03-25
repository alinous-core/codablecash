/*
 * FeeBananceTransactionScanner.cpp
 *
 *  Created on: 2023/02/28
 *      Author: iizuka
 */

#include "bc_memorypool/FeeBananceTransactionScanner.h"
#include "bc_memorypool/MemoryPool.h"
#include "bc_memorypool/MemPoolTransaction.h"
#include "bc_memorypool/BalanceTransferTrxPool.h"

#include "btree/BtreeReverseScanner.h"

#include "bc_trx_balance/BalanceTransferTransaction.h"

namespace codablecash {

FeeBananceTransactionScanner::FeeBananceTransactionScanner(MemPoolTransaction* memTrx, BtreeReverseScanner* feeTrxScanner)
		: AbstractMempoolTransactionScanner(memTrx, feeTrxScanner, memTrx->getPool()->getBalanceTransferTrxPool()){

}

FeeBananceTransactionScanner::~FeeBananceTransactionScanner() {

}

AbstractBalanceTransaction* FeeBananceTransactionScanner::next() const {
	return dynamic_cast<AbstractBalanceTransaction*>(AbstractMempoolTransactionScanner::next());
}

} /* namespace codablecash */
