/*
 * ArrayUtxoFinder.cpp
 *
 *  Created on: May 24, 2025
 *      Author: iizuka
 */

#include "bc_wallet_trx_base/ArrayUtxoFinder.h"

#include "bc_trx_balance/BalanceUtxo.h"

#include "bc_trx/UtxoId.h"

#include "bc_base/BalanceUnit.h"
namespace codablecash {

ArrayUtxoFinder::ArrayUtxoFinder() {
	this->utxoList = new ArrayList<BalanceUtxo>();

}

ArrayUtxoFinder::~ArrayUtxoFinder() {
	this->utxoList->deleteElements();
	delete this->utxoList;
}

void ArrayUtxoFinder::addUtxo(const BalanceUtxo *utxo) noexcept {
	this->utxoList->addElement(dynamic_cast<BalanceUtxo*>(utxo->copyData()));
}

BalanceUtxo* ArrayUtxoFinder::getBalanceUtxo(const UtxoId *utxoId) {
	BalanceUtxo* ret = nullptr;

	int maxLoop = this->utxoList->size();
	for(int i = 0; i != maxLoop; ++i){
		BalanceUtxo* utxo = this->utxoList->get(i);

		if(utxo->getId()->equals(utxoId)){
			ret = dynamic_cast<BalanceUtxo*>(utxo->copyData());
			break;
		}
	}

	return ret;
}

BalanceUnit ArrayUtxoFinder::getTotalAmount() const noexcept {
	BalanceUnit total(0L);

	int maxLoop = this->utxoList->size();
	for(int i = 0; i != maxLoop; ++i){
		BalanceUtxo* utxo = this->utxoList->get(i);
		BalanceUnit amount = utxo->getAmount();

		total += amount;
	}

	return total;
}

} /* namespace codablecash */
