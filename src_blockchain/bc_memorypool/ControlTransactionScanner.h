/*
 * ControlTransactionScanner.h
 *
 *  Created on: 2023/04/19
 *      Author: iizuka
 */

#ifndef BC_MEMORYPOOL_CONTROLTRANSACTIONSCANNER_H_
#define BC_MEMORYPOOL_CONTROLTRANSACTIONSCANNER_H_

#include "bc_memorypool/AbstractMempoolTransactionScanner.h"

namespace codablecash {

class AbstractControlTransaction;

class ControlTransactionScanner : public AbstractMempoolTransactionScanner {
public:
	ControlTransactionScanner(MemPoolTransaction* memTrx, BtreeReverseScanner* feeTrxScanner);
	virtual ~ControlTransactionScanner();

	AbstractControlTransaction* next() const;
};

} /* namespace codablecash */

#endif /* BC_MEMORYPOOL_CONTROLTRANSACTIONSCANNER_H_ */
