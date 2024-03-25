/*
 * P2pConnectionListeningThread.h
 *
 *  Created on: 2022/08/23
 *      Author: iizuka
 */

#ifndef P2PSERVER_P2PCONNECTIONLISTENINGTHREAD_H_
#define P2PSERVER_P2PCONNECTIONLISTENINGTHREAD_H_

#include "base_thread/AbstractThreadRunner.h"

#include "base/ArrayList.h"

namespace alinous {
class SysMutex;
}
using namespace alinous;

namespace codablecash {

class IServerSocket;
class P2pServer;
class P2pConnectionAcceptThread;


class P2pConnectionListeningThread : public alinous::AbstractThreadRunner {
public:
	P2pConnectionListeningThread(IServerSocket* srvSocket, P2pServer* p2pServer);
	virtual ~P2pConnectionListeningThread();

	void setContinueFlag(bool bl);
	bool isContinueFlag();

	void cleanAll() noexcept;

	/**
	 * for testing
	 */
	void setSocket(IServerSocket* srvSocket);

protected:
	virtual void process() noexcept;

private:
	void addThread(P2pConnectionAcceptThread* thread) noexcept;
	bool isThreadsEmpty() const noexcept;

	void cleanFinishedThreads() noexcept;
protected:
	P2pServer* p2pServer;
	IServerSocket* srvSocket;

	bool continueFlag;
	SysMutex* continueFlagMutex;

	ArrayList<P2pConnectionAcceptThread> list;
	SysMutex* listMutex;
};

} /* namespace codablecash */

#endif /* P2PSERVER_P2PCONNECTIONLISTENINGTHREAD_H_ */
