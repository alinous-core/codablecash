/*
 * ClientExecutor.h
 *
 *  Created on: 2023/10/05
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_NOTIFY_CLIENTEXECUTOR_H_
#define BC_P2P_CMD_CLIENT_NOTIFY_CLIENTEXECUTOR_H_

#include "pubsub/IPubsubCommandExecutor.h"

#include "base/ArrayList.h"

namespace alinous {
class SysMutex;
};
using namespace alinous;

namespace codablecash {

class IClientNotifyListner;
class AbstractCommandResponse;
class PubSubId;
class TransactionTransferData;

class ClientExecutor : public IPubsubCommandExecutor {
public:
	ClientExecutor();
	virtual ~ClientExecutor();

	void addListner(IClientNotifyListner* listner) noexcept;
	IClientNotifyListner* removeListner(IClientNotifyListner* listner) noexcept;

	AbstractCommandResponse* fireOnNewTransaction(const PubSubId *pubsubId, const TransactionTransferData* data);

private:
	SysMutex* mutex;
	ArrayList<IClientNotifyListner>* listners;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_NOTIFY_CLIENTEXECUTOR_H_ */
