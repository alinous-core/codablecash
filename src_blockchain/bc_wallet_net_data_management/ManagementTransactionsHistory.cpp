/*
 * ManagementTransactionsHistory.cpp
 *
 *  Created on: Jul 17, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_data_management/ManagementTransactionsHistory.h"

#include "bc_trx/AbstractBlockchainTransaction.h"
#include "bc_trx/TransactionId.h"


namespace codablecash {

ManagementTransactionsHistory::ManagementTransactionsHistory() {
	this->list = new ArrayList<AbstractBlockchainTransaction>();
	this->map = new HashMap<TransactionId, AbstractBlockchainTransaction>();
}

ManagementTransactionsHistory::~ManagementTransactionsHistory() {
	delete this->map;

	this->list->deleteElements();
	delete this->list;
}

void ManagementTransactionsHistory::reset() noexcept {
	this->map->clear();

	this->list->deleteElements();
	this->list->reset();
}

void ManagementTransactionsHistory::addTransaction(const AbstractBlockchainTransaction *trx) {
	AbstractBlockchainTransaction* newTrx= dynamic_cast<AbstractBlockchainTransaction*>(trx->copyData());
	this->list->addElement(newTrx);

	const TransactionId* trxId = trx->getTransactionId();
	this->map->put(trxId, newTrx);
}

int ManagementTransactionsHistory::historySize() const noexcept {
	return this->list->size();
}

const AbstractBlockchainTransaction* ManagementTransactionsHistory::getTransaction(int i) const noexcept {
	return this->list->get(i);
}

void ManagementTransactionsHistory::importOtherManagementTransactionsHistory(const ManagementTransactionsHistory *other) {
	int maxLoop = other->historySize();
	for(int i = 0; i != maxLoop; ++i){
		const AbstractBlockchainTransaction* trx = other->getTransaction(i);

		addTransaction(trx);
	}
}

} /* namespace codablecash */
