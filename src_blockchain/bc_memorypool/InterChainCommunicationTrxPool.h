/*
 * InterChainCommunicationTrxPool.h
 *
 *  Created on: 2023/11/30
 *      Author: iizuka
 */

#ifndef BC_MEMORYPOOL_INTERCHAINCOMMUNICATIONTRXPOOL_H_
#define BC_MEMORYPOOL_INTERCHAINCOMMUNICATIONTRXPOOL_H_

#include "bc_memorypool/AbstractTrxMemoryPool.h"

namespace codablecash {

class AbstractInterChainCommunicationTansaction;
class TransactionId;

class InterChainCommunicationTrxPool : public AbstractTrxMemoryPool {
public:
	explicit InterChainCommunicationTrxPool(const File* baseDir);
	virtual ~InterChainCommunicationTrxPool();

	void putTransaction(const AbstractInterChainCommunicationTansaction* trx);
	void removeTransaction(const AbstractInterChainCommunicationTansaction* trx);

	AbstractInterChainCommunicationTansaction* getInterChainCommunicationTansaction(const TransactionId* trxId) const;

};

} /* namespace codablecash */

#endif /* BC_MEMORYPOOL_INTERCHAINCOMMUNICATIONTRXPOOL_H_ */
