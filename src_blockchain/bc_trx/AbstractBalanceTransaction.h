/*
 * AbstractBalanceTransaction.h
 *
 *  Created on: 2023/03/01
 *      Author: iizuka
 */

#ifndef BC_TRX_ABSTRACTBALANCETRANSACTION_H_
#define BC_TRX_ABSTRACTBALANCETRANSACTION_H_

#include "bc_trx/AbstractBlockchainTransaction.h"

namespace codablecash {

class BalanceUnit;
class AbstractUtxo;

class AbstractBalanceTransaction: public AbstractBlockchainTransaction {
public:
	AbstractBalanceTransaction(const AbstractBalanceTransaction& inst);
	AbstractBalanceTransaction();
	virtual ~AbstractBalanceTransaction();
};

} /* namespace codablecash */

#endif /* BC_TRX_ABSTRACTBALANCETRANSACTION_H_ */
