/*
 * FeeBananceTransactionScanner.h
 *
 *  Created on: 2023/02/28
 *      Author: iizuka
 */

#ifndef BC_MEMORYPOOL_FEEBANANCETRANSACTIONSCANNER_H_
#define BC_MEMORYPOOL_FEEBANANCETRANSACTIONSCANNER_H_

#include "bc_memorypool/AbstractMempoolTransactionScanner.h"

namespace codablecash {

class AbstractBalanceTransaction;

class FeeBananceTransactionScanner : public AbstractMempoolTransactionScanner {
public:
	explicit FeeBananceTransactionScanner(MemPoolTransaction* memTrx, BtreeReverseScanner* feeTrxScanner);
	virtual ~FeeBananceTransactionScanner();

	AbstractBalanceTransaction* next() const;
};

} /* namespace codablecash */

#endif /* BC_MEMORYPOOL_FEEBANANCETRANSACTIONSCANNER_H_ */
