/*
 * P2pRequestQueueProcessor.h
 *
 *  Created on: 2023/09/05
 *      Author: iizuka
 */

#ifndef BC_P2P_PROCESSOR_P2PREQUESTQUEUEPROCESSOR_H_
#define BC_P2P_PROCESSOR_P2PREQUESTQUEUEPROCESSOR_H_

#include "pubsub/IPubsubCommandListner.h"

namespace alinous {
class File;
class SysMutex;
class SynchronizedLock;
}
using namespace alinous;

namespace codablecash {

class CommandQueue;
class NetworkTransferProcessor;
class P2pRequestQueueProcessorThread;
class CommandQueueData;
class BlochchainP2pManager;
class ISystemLogger;
class CommandQueueData;
class CodablecashNodeInstance;

class P2pRequestQueueProcessor : public IPubsubCommandListner {
public:
	static const constexpr wchar_t* THREAD_NAME{L"QP"};

	P2pRequestQueueProcessor(const File* baseDir, BlochchainP2pManager* p2pManager, NetworkTransferProcessor* processor, CodablecashNodeInstance *inst, ISystemLogger* logger);
	virtual ~P2pRequestQueueProcessor();

	void createBlankDatabase();
	void open(bool suspend);
	void close();

	void put(const CommandQueueData* data);

	bool isEmpty() const;
	CommandQueueData* fetchFirst() const;

	virtual void fireExecuteCommand(const PubSubId* pubsubId, const AbstractPubSubCommand* cmd){};
	virtual void fireOnExeptionOnSubscriber(const PubSubId* pubsubId, const Exception* e){};
	virtual void fireOnSubscribeEnds(const PubSubId* pubsubId){};

	virtual IPubsubCommandExecutor* getExecutor() const noexcept;

	CommandQueue* getCommandQueue() const noexcept {
		return this->queue;
	}

	void setSuspend(bool suspend);
	void __setSuspend(bool suspend);
	bool __isSuspended() const noexcept;

	SynchronizedLock* getSynchronizedLock() const noexcept;

private:
	SysMutex* mutex;
	CommandQueue* queue;
	ISystemLogger* logger;

	BlochchainP2pManager* p2pManager;
	NetworkTransferProcessor* processor;
	P2pRequestQueueProcessorThread* thread;

	CodablecashNodeInstance *inst;
};

} /* namespace codablecash */

#endif /* BC_P2P_PROCESSOR_P2PREQUESTQUEUEPROCESSOR_H_ */
