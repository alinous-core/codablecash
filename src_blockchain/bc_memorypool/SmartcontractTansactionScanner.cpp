/*
 * SmartcontractTansactionScanner.cpp
 *
 *  Created on: 2023/12/02
 *      Author: iizuka
 */

#include "bc_memorypool/SmartcontractTansactionScanner.h"
#include "bc_memorypool/MemPoolTransaction.h"
#include "bc_memorypool/MemoryPool.h"
#include "bc_memorypool/SmartcontractTrxPool.h"

#include "btree/BtreeReverseScanner.h"

#include "transaction/AbstractSmartcontractTransaction.h"

namespace codablecash {

SmartcontractTansactionScanner::SmartcontractTansactionScanner(MemPoolTransaction* memTrx, BtreeReverseScanner* feeTrxScanner)
		: AbstractMempoolTransactionScanner(memTrx, feeTrxScanner, memTrx->getPool()->getSmartcontractTrxPool()){

}

SmartcontractTansactionScanner::~SmartcontractTansactionScanner() {

}

AbstractSmartcontractTransaction* SmartcontractTansactionScanner::next() const {
	return dynamic_cast<AbstractSmartcontractTransaction*>(AbstractMempoolTransactionScanner::next());
}

} /* namespace codablecash */
