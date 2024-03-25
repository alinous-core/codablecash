/*
 * AbstractBalanceTransaction.cpp
 *
 *  Created on: 2023/03/01
 *      Author: iizuka
 */

#include "bc_trx/AbstractBalanceTransaction.h"

namespace codablecash {


AbstractBalanceTransaction::AbstractBalanceTransaction(const AbstractBalanceTransaction &inst)
				: AbstractBlockchainTransaction(inst) {
}

AbstractBalanceTransaction::AbstractBalanceTransaction()
				: AbstractBlockchainTransaction(){

}

AbstractBalanceTransaction::~AbstractBalanceTransaction() {

}

} /* namespace codablecash */
