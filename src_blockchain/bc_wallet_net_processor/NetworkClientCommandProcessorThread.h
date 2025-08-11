/*
 * NetworkClientCommandProcessorThread.h
 *
 *  Created on: Mar 26, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_PROCESSOR_NETWORKCLIENTCOMMANDPROCESSORTHREAD_H_
#define BC_WALLET_NET_PROCESSOR_NETWORKCLIENTCOMMANDPROCESSORTHREAD_H_

#include "base_thread/AbstractThreadRunner.h"

namespace alinous {
class SynchronizedLock;
}
using namespace alinous;

namespace codablecash {

class ISystemLogger;
class ClientCommandsQueueProcessor;
class AbstractClientQueueCommand;
class NetworkWallet;

class NetworkClientCommandProcessorThread : public AbstractThreadRunner {
public:
	static const constexpr int STATUS_STOPPED{0};
	static const constexpr int STATUS_WAITING{1};
	static const constexpr int STATUS_RUNNING{2};

	NetworkClientCommandProcessorThread(NetworkWallet* wallet, ClientCommandsQueueProcessor* queue, bool suspend, ISystemLogger* logger, const UnicodeString* name);
	virtual ~NetworkClientCommandProcessorThread();

	void setRunning(bool bl) noexcept;
	bool isRunning() const noexcept;

	int getStatus() const noexcept;

	void __setSuspend(bool suspend);

	bool isSuspend() const;
	bool __isSuspended() const noexcept {
		return this->suspend;
	}

	SynchronizedLock* getSynchronizedLock() const noexcept {
		return this->lock;
	}

	void putCommnad(const AbstractClientQueueCommand* cmd);

protected:
	virtual void process() noexcept;

	void processCommand(const AbstractClientQueueCommand* cmd) const;

private:
	ClientCommandsQueueProcessor* queue;
	ISystemLogger* logger;

	volatile int status;
	volatile bool running;
	volatile bool suspend;

	SynchronizedLock* lock;

	NetworkWallet* wallet;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_PROCESSOR_NETWORKCLIENTCOMMANDPROCESSORTHREAD_H_ */
