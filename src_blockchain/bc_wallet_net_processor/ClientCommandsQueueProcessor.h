/*
 * ClientCommandsQueueProcessor.h
 *
 *  Created on: Mar 26, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_PROCESSOR_CLIENTCOMMANDSQUEUEPROCESSOR_H_
#define BC_WALLET_NET_PROCESSOR_CLIENTCOMMANDSQUEUEPROCESSOR_H_

namespace alinous {
class File;
class SysMutex;
class SynchronizedLock;
}
using namespace alinous;

namespace codablecash {

class ClientCommandsQueue;
class AbstractClientCommand;
class NetworkClientCommandProcessorThread;
class ISystemLogger;
class NetworkWallet;

class ClientCommandsQueueProcessor {
public:
	static const constexpr wchar_t* THREAD_NAME{L"CLQ"};

	ClientCommandsQueueProcessor(NetworkWallet* wallet, const File* baseDir, ISystemLogger* logger);
	virtual ~ClientCommandsQueueProcessor();

	void createBlankDatabase();
	void open(bool suspend);
	void close();

	bool isEmpty() const;
	AbstractClientCommand* fetchFirst() const;

	bool __isSuspended() const;
	void __setSuspend(bool suspend);

	SynchronizedLock* getSynchrinizedLock() const noexcept;

	void addCommand(const AbstractClientCommand *cmd);

private:
	ISystemLogger* logger;

	SysMutex* mutex;
	ClientCommandsQueue* queue;

	NetworkClientCommandProcessorThread* thread;

	NetworkWallet* wallet;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_PROCESSOR_CLIENTCOMMANDSQUEUEPROCESSOR_H_ */
