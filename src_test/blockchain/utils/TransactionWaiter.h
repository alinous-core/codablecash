/*
 * TransactionWaiter.h
 *
 *  Created on: 2023/11/13
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_UTILS_TRANSACTIONWAITER_H_
#define BLOCKCHAIN_UTILS_TRANSACTIONWAITER_H_

#include "bc_p2p_cmd_client_notify/IClientNotifyListner.h"

namespace alinous {
class SynchronizedLock;
}
using namespace alinous;

namespace codablecash {

class TransactionId;

class TransactionWaiter : public IClientNotifyListner {
public:
	explicit TransactionWaiter(const TransactionId* trxId);
	virtual ~TransactionWaiter();

	virtual void onNewTransaction(const PubSubId *pubsubId, const TransactionTransferData* data);
	virtual void onBlockMined(const PubSubId *pubsubId, const BlockHeaderTransferData *blockcheader);

	void wait();

private:
	const TransactionId* trxId;
	SynchronizedLock* lock;
	bool checked;
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_UTILS_TRANSACTIONWAITER_H_ */
