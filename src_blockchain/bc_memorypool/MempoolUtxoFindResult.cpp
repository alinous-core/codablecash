/*
 * MempoolUtxoFindResult.cpp
 *
 *  Created on: 2023/05/12
 *      Author: iizuka
 */

#include "bc_memorypool/MempoolUtxoFindResult.h"

#include "bc_trx/TransactionId.h"
#include "bc_trx/AbstractUtxo.h"

namespace codablecash {

MempoolUtxoFindResult::MempoolUtxoFindResult(const TransactionId* trxId, const AbstractUtxo* utxo) {
	this->trxId = dynamic_cast<TransactionId*>(trxId->copyData());
	this->utxo = dynamic_cast<AbstractUtxo*>(utxo->copyData());
}

MempoolUtxoFindResult::~MempoolUtxoFindResult() {
	delete this->trxId;
	delete this->utxo;
}

} /* namespace codablecash */
