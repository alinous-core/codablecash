/*
 * NetWalletAccountUtxoArray.cpp
 *
 *  Created on: Jul 30, 2025
 *      Author: iizuka
 */

#include "bc_wallet_trx_base/NetWalletAccountUtxoArray.h"

#include "bc_trx_balance/BalanceUtxo.h"


namespace codablecash {

NetWalletAccountUtxoArray::NetWalletAccountUtxoArray() {
	this->list = new ArrayList<BalanceUtxo>();
	this->pos = 0;
}

NetWalletAccountUtxoArray::~NetWalletAccountUtxoArray() {
	this->list->deleteElements();
	delete this->list;
}

void NetWalletAccountUtxoArray::addBalanceUtxo(const BalanceUtxo *utxo) noexcept {
	this->list->addElement(dynamic_cast<BalanceUtxo*>(utxo->copyData()));
}

bool NetWalletAccountUtxoArray::hasNext() noexcept {
	return this->list->size() > this->pos;
}

const BalanceUtxo* NetWalletAccountUtxoArray::next() noexcept {
	return this->list->get(this->pos++);
}

} /* namespace codablecash */
