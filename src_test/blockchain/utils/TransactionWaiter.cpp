/*
 * TransactionWaiter.cpp
 *
 *  Created on: 2023/11/13
 *      Author: iizuka
 */

#include "TransactionWaiter.h"

#include "data_history_data/TransactionTransferData.h"

#include "base_thread/StackUnlocker.h"
#include "base_thread/SynchronizedLock.h"

#include "bc_trx/TransactionId.h"
#include "bc_trx/AbstractBlockchainTransaction.h"

namespace codablecash {

TransactionWaiter::TransactionWaiter(const TransactionId* trxId) {
	this->trxId = trxId;
	this->lock = new SynchronizedLock();
	this->checked = false;
}

TransactionWaiter::~TransactionWaiter() {
	this->trxId = nullptr;
	delete this->lock;
}

void TransactionWaiter::onNewTransaction(const PubSubId *pubsubId, const TransactionTransferData *data) {
	AbstractBlockchainTransaction* trx = data->getTransaction();
	const TransactionId* id = trx->getTransactionId();

	{
		StackUnlocker __lock(this->lock, __FILE__, __LINE__);

		if(this->trxId->equals(id)){
			this->checked = true;
		}

		this->lock->notifyAll();
	}
}

void TransactionWaiter::wait() {
	StackUnlocker __lock(this->lock, __FILE__, __LINE__);

	while(!this->checked){
		this->lock->wait();
	}
}

} /* namespace codablecash */
