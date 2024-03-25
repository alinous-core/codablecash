/*
 * AbstractSmartcontractTransaction.h
 *
 *  Created on: 2023/11/30
 *      Author: iizuka
 */

#ifndef BC_SMARTCONTRACT_ABSTRACTSMARTCONTRACTTRANSACTION_H_
#define BC_SMARTCONTRACT_ABSTRACTSMARTCONTRACTTRANSACTION_H_

#include "bc_trx/AbstractBlockchainTransaction.h"

namespace codablecash {

class AbstractSmartcontractTransaction : public AbstractBlockchainTransaction {
public:
	AbstractSmartcontractTransaction(const AbstractSmartcontractTransaction& inst);
	AbstractSmartcontractTransaction();
	virtual ~AbstractSmartcontractTransaction();
};

} /* namespace codablecash */

#endif /* BC_SMARTCONTRACT_ABSTRACTSMARTCONTRACTTRANSACTION_H_ */
