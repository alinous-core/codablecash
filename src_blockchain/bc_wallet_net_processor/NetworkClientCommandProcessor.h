/*
 * NetworkClientCommandProcessor.h
 *
 *  Created on: Mar 21, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_NETWORKCLIENTCOMMANDPROCESSOR_H_
#define BC_WALLET_NET_NETWORKCLIENTCOMMANDPROCESSOR_H_

#include "pubsub/IPubsubCommandListner.h"

namespace alinous {
class File;
class SynchronizedLock;
}
using namespace alinous;

namespace codablecash {

class NetworkWallet;
class ClientExecutor;
class NetworkClientCommandListner;
class PendingClientCommandsQueue;
class ClientCommandsQueueProcessor;
class ClientCommandsQueueData;
class ISystemLogger;
class AbstractClientCommand;

class NetworkClientCommandProcessor : public IPubsubCommandListner {
public:
	static const constexpr wchar_t* PROCESSER_DIR{L"cmd_processor"};

	explicit NetworkClientCommandProcessor(NetworkWallet* networkWallet, ISystemLogger* logger);
	virtual ~NetworkClientCommandProcessor();

	void init();
	void createBlank();

	void startProcessors(bool suspend);
	void resumeRequestProcessor();
	void shurdownProcessors() noexcept;

	SynchronizedLock* getQueueSynchrinizedLock() const noexcept;
	bool __isSuspended() const;

	virtual void fireExecuteCommand(const PubSubId* pubsubId, const AbstractPubSubCommand* cmd);
	virtual void fireOnExeptionOnSubscriber(const PubSubId* pubsubId, const Exception* e);
	virtual void fireOnSubscribeEnds(const PubSubId* pubsubId);

	virtual IPubsubCommandExecutor* getExecutor() const noexcept;

	void addPendingQueue(const ClientCommandsQueueData* queueData);
	void addClientCommand(const AbstractClientCommand* clientCommnad);

private:
	ClientCommandsQueueData* fetchFirstPendingData(SynchronizedLock* lock) const;
	bool isPendingQueueEmptyWithResume(SynchronizedLock* lock) const;
	void processPendingData(ClientCommandsQueueData* data);


private:
	ISystemLogger* logger;
	NetworkWallet* networkWallet;

	ClientExecutor* clientExec;
	NetworkClientCommandListner* listner; // extends public IClientNotifyListner

	ClientCommandsQueueProcessor* queueProcessor;
	PendingClientCommandsQueue* pendingQueue;

};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_NETWORKCLIENTCOMMANDPROCESSOR_H_ */
