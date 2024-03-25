/*
 * SmartcontractTansactionScanner.h
 *
 *  Created on: 2023/12/02
 *      Author: iizuka
 */

#ifndef BC_MEMORYPOOL_SMARTCONTRACTTANSACTIONSCANNER_H_
#define BC_MEMORYPOOL_SMARTCONTRACTTANSACTIONSCANNER_H_

#include "bc_memorypool/AbstractMempoolTransactionScanner.h"

namespace codablecash {

class AbstractSmartcontractTransaction;

class SmartcontractTansactionScanner : public AbstractMempoolTransactionScanner {
public:
	SmartcontractTansactionScanner(MemPoolTransaction* memTrx, BtreeReverseScanner* feeTrxScanner);
	virtual ~SmartcontractTansactionScanner();

	AbstractSmartcontractTransaction* next() const;
};

} /* namespace codablecash */

#endif /* BC_MEMORYPOOL_SMARTCONTRACTTANSACTIONSCANNER_H_ */
