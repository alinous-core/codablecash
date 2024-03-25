/*
 * AbstractInterChainCommunicationTansaction.h
 *
 *  Created on: 2023/11/30
 *      Author: iizuka
 */

#ifndef BC_TRX_ABSTRACTINTERCHAINCOMMUNICATIONTANSACTION_H_
#define BC_TRX_ABSTRACTINTERCHAINCOMMUNICATIONTANSACTION_H_

#include "bc_trx/AbstractBlockchainTransaction.h"

namespace codablecash {

class AbstractInterChainCommunicationTansaction : public AbstractBlockchainTransaction {
public:
	AbstractInterChainCommunicationTansaction(const AbstractInterChainCommunicationTansaction& inst);
	AbstractInterChainCommunicationTansaction();
	virtual ~AbstractInterChainCommunicationTansaction();
};

} /* namespace codablecash */

#endif /* BC_TRX_ABSTRACTINTERCHAINCOMMUNICATIONTANSACTION_H_ */
