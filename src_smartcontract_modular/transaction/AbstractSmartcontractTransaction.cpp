/*
 * AbstractSmartcontractTransaction.cpp
 *
 *  Created on: 2023/11/30
 *      Author: iizuka
 */

#include "transaction/AbstractSmartcontractTransaction.h"

namespace codablecash {

AbstractSmartcontractTransaction::AbstractSmartcontractTransaction(const AbstractSmartcontractTransaction &inst)
						: AbstractBlockchainTransaction(inst) {
}

AbstractSmartcontractTransaction::AbstractSmartcontractTransaction() : AbstractBlockchainTransaction() {

}

AbstractSmartcontractTransaction::~AbstractSmartcontractTransaction() {

}

} /* namespace codablecash */
