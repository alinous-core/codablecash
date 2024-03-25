/*
 * InterChainCommunicationTransactionScanner.h
 *
 *  Created on: 2023/12/02
 *      Author: iizuka
 */

#ifndef BC_MEMORYPOOL_INTERCHAINCOMMUNICATIONTRANSACTIONSCANNER_H_
#define BC_MEMORYPOOL_INTERCHAINCOMMUNICATIONTRANSACTIONSCANNER_H_

#include "bc_memorypool/AbstractMempoolTransactionScanner.h"

namespace codablecash {

class AbstractInterChainCommunicationTansaction;

class InterChainCommunicationTransactionScanner : public AbstractMempoolTransactionScanner {
public:
	InterChainCommunicationTransactionScanner(MemPoolTransaction* memTrx, BtreeReverseScanner* feeTrxScanner);
	virtual ~InterChainCommunicationTransactionScanner();

	AbstractInterChainCommunicationTansaction* next() const;
};

} /* namespace codablecash */

#endif /* BC_MEMORYPOOL_INTERCHAINCOMMUNICATIONTRANSACTIONSCANNER_H_ */
