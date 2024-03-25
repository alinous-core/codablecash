/*
 * AbstractInterChainCommunicationTansaction.cpp
 *
 *  Created on: 2023/11/30
 *      Author: iizuka
 */

#include "bc_trx/AbstractInterChainCommunicationTansaction.h"

namespace codablecash {

AbstractInterChainCommunicationTansaction::AbstractInterChainCommunicationTansaction(const AbstractInterChainCommunicationTansaction &inst)
		: AbstractBlockchainTransaction(inst){
}

AbstractInterChainCommunicationTansaction::AbstractInterChainCommunicationTansaction()
		: AbstractBlockchainTransaction() {

}

AbstractInterChainCommunicationTansaction::~AbstractInterChainCommunicationTansaction() {

}

} /* namespace codablecash */
