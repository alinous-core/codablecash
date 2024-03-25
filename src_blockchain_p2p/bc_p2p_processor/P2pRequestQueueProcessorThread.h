/*
 * P2pRequestQueueProcessorThread.h
 *
 *  Created on: 2023/09/25
 *      Author: iizuka
 */

#ifndef BC_P2P_PROCESSOR_P2PREQUESTQUEUEPROCESSORTHREAD_H_
#define BC_P2P_PROCESSOR_P2PREQUESTQUEUEPROCESSORTHREAD_H_

#include "base_thread/AbstractThreadRunner.h"

namespace alinous {
class SynchronizedLock;
}
using namespace alinous;


namespace codablecash {

class BlochchainP2pManager;
class P2pRequestQueueProcessor;
class ISystemLogger;
class CommandQueueData;

class P2pRequestQueueProcessorThread : public AbstractThreadRunner {
public:
	static const constexpr int STATUS_STOPPED{0};
	static const constexpr int STATUS_WAITING{1};
	static const constexpr int STATUS_RUNNING{2};

	P2pRequestQueueProcessorThread(BlochchainP2pManager* p2pManager, P2pRequestQueueProcessor* queue, bool suspend, ISystemLogger* logger, const UnicodeString* name);
	virtual ~P2pRequestQueueProcessorThread();

	void setRunning(bool bl) noexcept;
	bool isRunning() const noexcept;

//	void setSuspend(bool suspend);
	void __setSuspend(bool suspend);
	bool __isSuspended() const noexcept {
		return this->suspend;
	}

	int getStatus() const noexcept;

	//void notifyAll();
	SynchronizedLock* getSynchronizedLock() const noexcept {
		return this->lock;
	}


protected:
	virtual void process() noexcept;

	void processCommand(CommandQueueData* cmd);

private:
	BlochchainP2pManager* p2pManager;
	P2pRequestQueueProcessor* queue;
	ISystemLogger* logger;

	volatile int status;
	volatile bool running;
	volatile bool suspend;

	SynchronizedLock* lock;
};

} /* namespace codablecash */

#endif /* BC_P2P_PROCESSOR_P2PREQUESTQUEUEPROCESSORTHREAD_H_ */
