/*
 * AbstractFinalizerTransaction.cpp
 *
 *  Created on: 2023/04/15
 *      Author: iizuka
 */

#include "bc_finalizer_trx/AbstractFinalizerTransaction.h"

namespace codablecash {

AbstractFinalizerTransaction::AbstractFinalizerTransaction(	const AbstractFinalizerTransaction &inst)
		: AbstractControlTransaction(inst) {
}

AbstractFinalizerTransaction::AbstractFinalizerTransaction()
	: AbstractControlTransaction() {

}

AbstractFinalizerTransaction::~AbstractFinalizerTransaction() {

}

} /* namespace codablecash */
