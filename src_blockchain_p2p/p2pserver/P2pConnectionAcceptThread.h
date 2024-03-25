/*
 * P2pConnectionAcceptThread.h
 *
 *  Created on: 2022/08/26
 *      Author: iizuka
 */

#ifndef P2PSERVER_P2PCONNECTIONACCEPTTHREAD_H_
#define P2PSERVER_P2PCONNECTIONACCEPTTHREAD_H_

#include "base_thread/AbstractThreadRunner.h"

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class IClientSocket;
class P2pServer;

class P2pConnectionAcceptThread : public alinous::AbstractThreadRunner {
public:
	P2pConnectionAcceptThread(IClientSocket* client, P2pServer* p2pServer);
	virtual ~P2pConnectionAcceptThread();

	virtual void process() noexcept;

	bool isDone() const noexcept {
		return done;
	}

	IClientSocket* delicateConnection();

	P2pServer* getP2pServer() const noexcept {
		return p2pServer;
	}

private:
	ByteBuffer* readAll(IClientSocket* client, int size);

private:
	IClientSocket* client;
	P2pServer* p2pServer;

	bool done;
};

} /* namespace codablecash */

#endif /* P2PSERVER_P2PCONNECTIONACCEPTTHREAD_H_ */
