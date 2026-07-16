/*
 * TransactionUtxoChecker.cpp
 *
 *  Created on: Jun 5, 2026
 *      Author: iizuka
 */

#include "TransactionUtxoChecker.h"

#include "bc_trx/AbstractBlockchainTransaction.h"
#include "bc_trx/AbstractUtxoReference.h"
#include "bc_trx/UtxoId.h"


namespace codablecash {

TransactionUtxoChecker::TransactionUtxoChecker() {
	this->list = new ArrayList<AbstractBlockchainTransaction>();
}

TransactionUtxoChecker::~TransactionUtxoChecker() {
	this->list->deleteElements();
	delete this->list;
}

bool TransactionUtxoChecker::checkTransaction(const AbstractBlockchainTransaction *trx) {
	{
		int maxLoop = trx->getUtxoReferenceSize();
		for(int i = 0; i != maxLoop; ++i){
			AbstractUtxoReference* utxoRef = trx->getUtxoReference(i);

			if(!checkDoubleUseUtxo(utxoRef)){
				addTrx(trx);
				return false;
			}
		}
	}

	addTrx(trx);
	return true;
}

bool TransactionUtxoChecker::checkDoubleUseUtxo(const AbstractUtxoReference *utxoRef) {
	const UtxoId* id = utxoRef->getUtxoId();

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractBlockchainTransaction* trx = this->list->get(i);

		if(hasUtxoRef(trx, id)){
			return false;
		}
	}

	return true;
}

bool TransactionUtxoChecker::hasUtxoRef(const AbstractBlockchainTransaction *trx, const UtxoId* id) {
	int maxLoop = trx->getUtxoReferenceSize();
	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxoReference* utxoRef = trx->getUtxoReference(i);

		const UtxoId* utxoId = utxoRef->getUtxoId();
		if(utxoId->equals(id)){
			return true;
		}
	}
	return false;
}

void TransactionUtxoChecker::addTrx(const AbstractBlockchainTransaction *trx) {
	AbstractBlockchainTransaction* t = dynamic_cast<AbstractBlockchainTransaction*>(trx->copyData());
	this->list->addElement(t);
}

} /* namespace codablecash */
