/*
 * DummyClientListner.h
 *
 *  Created on: 2023/10/11
 *      Author: iizuka
 */

#ifndef P2P_P2PSERVER_DUMMY_DUMMYCLIENTLISTNER_H_
#define P2P_P2PSERVER_DUMMY_DUMMYCLIENTLISTNER_H_

#include "bc_p2p_cmd_client_notify/IClientNotifyListner.h"

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class DummyClientListner : public IClientNotifyListner {
public:
	DummyClientListner();
	virtual ~DummyClientListner();

	virtual void onNewTransaction(const PubSubId *pubsubId, const TransactionTransferData* data);

	int size() const noexcept;

	void waitUntilCount(int trxCount);

private:
	ArrayList<TransactionTransferData>* list;
};

} /* namespace codablecash */

#endif /* P2P_P2PSERVER_DUMMY_DUMMYCLIENTLISTNER_H_ */
