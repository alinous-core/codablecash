/*
 * DummyServer.h
 *
 *  Created on: 2023/01/16
 *      Author: iizuka
 */

#ifndef P2P_P2PSERVER_DUMMYSERVER_H_
#define P2P_P2PSERVER_DUMMYSERVER_H_

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class IServerSocket;
class P2pConnectionListeningThread;

class DummyServer {
public:
	DummyServer();
	virtual ~DummyServer();

	void startIpV6Listening(const UnicodeString* host, int port, P2pConnectionListeningThread* thread);


private:
	void waitForStarted();

private:
	P2pConnectionListeningThread* thread;
	IServerSocket* srvSocket;
	int portSelf;
};

} /* namespace codablecash */

#endif /* P2P_P2PSERVER_DUMMYSERVER_H_ */
