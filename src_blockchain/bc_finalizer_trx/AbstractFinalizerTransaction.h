/*
 * AbstractFinalizerTransaction.h
 *
 *  Created on: 2023/04/15
 *      Author: iizuka
 */

#ifndef BC_FINALIZER_TRX_ABSTRACTFINALIZERTRANSACTION_H_
#define BC_FINALIZER_TRX_ABSTRACTFINALIZERTRANSACTION_H_

#include "bc_trx/AbstractControlTransaction.h"

namespace codablecash {

class AbstractFinalizerTransaction: public AbstractControlTransaction {
public:
	AbstractFinalizerTransaction(const AbstractFinalizerTransaction& inst);
	AbstractFinalizerTransaction();
	virtual ~AbstractFinalizerTransaction();
};

} /* namespace codablecash */

#endif /* BC_FINALIZER_TRX_ABSTRACTFINALIZERTRANSACTION_H_ */
