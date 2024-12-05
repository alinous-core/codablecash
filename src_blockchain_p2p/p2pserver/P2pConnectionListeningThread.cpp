/*
 * P2pConnectionListeningThread.cpp
 *
 *  Created on: 2022/08/23
 *      Author: iizuka
 */

#include <cstdint>

#include "p2pserver/P2pConnectionListeningThread.h"

#include "ipconnect/IServerSocket.h"
#include "ipconnect/IClientSocket.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "p2pserver/P2pConnectionAcceptThread.h"

#include "base_thread/SysMutex.h"
#include "base_thread/StackUnlocker.h"

#include "base_io/ByteBuffer.h"

#include "osenv/funcs.h"


namespace codablecash {

P2pConnectionListeningThread::P2pConnectionListeningThread(IServerSocket* srvSocket, P2pServer* p2pServer, const UnicodeString* name)
		: AbstractThreadRunner(name) {
	this->srvSocket = srvSocket;
	this->p2pServer = p2pServer;
	this->continueFlag = false;
	this->continueFlagMutex = new SysMutex();
	this->listMutex = new SysMutex();
}

P2pConnectionListeningThread::~P2pConnectionListeningThread() {
	cleanAll();

	this->srvSocket = nullptr;

	delete this->continueFlagMutex;
	delete this->listMutex;
}

void P2pConnectionListeningThread::process() noexcept {
	setContinueFlag(true);

	while(isContinueFlag()){
		int ret = this->srvSocket->poolAccept(0, 50000);
		if(ret > 0){
			IClientSocket* client = this->srvSocket->accept(); __STP(client);

			UnicodeString thname(L"");
			if(this->name != nullptr){
				thname.append(this->name);
				thname.append(L"_");
			}
			thname.append(L"ACTH");

			P2pConnectionAcceptThread* thread = new P2pConnectionAcceptThread(__STP_MV(client), this->p2pServer, &thname);

			thread->start();
			addThread(thread);

			cleanFinishedThreads();
		}
	}

	this->srvSocket->close();
}

void P2pConnectionListeningThread::addThread(P2pConnectionAcceptThread *thread) noexcept {
	StackUnlocker __lock(this->listMutex, __FILE__, __LINE__);
	this->list.addElement(thread);
}

bool P2pConnectionListeningThread::isThreadsEmpty() const noexcept {
	StackUnlocker __lock(this->listMutex, __FILE__, __LINE__);
	return this->list.isEmpty();
}

void P2pConnectionListeningThread::cleanAll() noexcept {
	do{
		cleanFinishedThreads();
		Os::usleep(100*1000);
	}
	while(!isThreadsEmpty());
}

void P2pConnectionListeningThread::setContinueFlag(bool bl) {
	StackUnlocker __lock(this->continueFlagMutex, __FILE__, __LINE__);

	this->continueFlag = bl;
}

bool P2pConnectionListeningThread::isContinueFlag() {
	StackUnlocker __lock(this->continueFlagMutex, __FILE__, __LINE__);

	bool ret = this->continueFlag;
	return ret;
}

void P2pConnectionListeningThread::cleanFinishedThreads() noexcept {
	StackUnlocker __lock(this->listMutex, __FILE__, __LINE__);

	ArrayList<P2pConnectionAcceptThread> removelList;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		P2pConnectionAcceptThread* th = this->list.get(i);

		if(th->isDone()){
			th->join();
			removelList.addElement(th);
		}
	}

	maxLoop = removelList.size();
	for(int i = 0; i != maxLoop; ++i){
		P2pConnectionAcceptThread* th = removelList.get(i);

		this->list.removeByObj(th);
	}

	removelList.deleteElements();
}

void P2pConnectionListeningThread::setSocket(IServerSocket *srvSocket) {
	this->srvSocket =srvSocket;
}

} /* namespace codablecash */
