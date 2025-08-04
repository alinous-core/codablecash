/*
 * ManagedUtxoCacheRecord.cpp
 *
 *  Created on: Jul 22, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_data_management/ManagedUtxoCacheRecord.h"

#include "bc_trx/AbstractUtxo.h"
#include "bc_trx/TransactionId.h"

namespace codablecash {

ManagedUtxoCacheRecord::ManagedUtxoCacheRecord() {
	this->utxo = nullptr;
	this->type = FINALIZED;
	this->transactionId = nullptr;
}

ManagedUtxoCacheRecord::~ManagedUtxoCacheRecord() {
	delete this->utxo;
	delete this->transactionId;
}

void ManagedUtxoCacheRecord::setUtxo(const AbstractUtxo *utxo) {
	delete this->utxo, this->utxo = nullptr;

	this->utxo = dynamic_cast<AbstractUtxo*>(utxo->copyData());
}

void ManagedUtxoCacheRecord::setType(uint8_t type) {
	this->type = type;
}

void ManagedUtxoCacheRecord::setTransactionId(const TransactionId *trxId) {
	delete this->transactionId, this->transactionId = nullptr;

	this->transactionId = dynamic_cast<TransactionId*>(trxId->copyData());
}

const UtxoId* ManagedUtxoCacheRecord::getUtxoId() const noexcept {
	return this->utxo->getId();
}

} /* namespace codablecash */
