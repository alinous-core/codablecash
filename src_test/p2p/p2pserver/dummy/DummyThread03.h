/*
 * DummyThread03.h
 *
 *  Created on: 2023/01/16
 *      Author: iizuka
 */

#ifndef P2P_P2PSERVER_DUMMY_DUMMYTHREAD03_H_
#define P2P_P2PSERVER_DUMMY_DUMMYTHREAD03_H_

#include "p2pserver/P2pConnectionListeningThread.h"

namespace codablecash {

class DummyThread03 : public P2pConnectionListeningThread {
public:
	DummyThread03();
	virtual ~DummyThread03();

protected:
	virtual void process() noexcept;
};

} /* namespace codablecash */

#endif /* P2P_P2PSERVER_DUMMY_DUMMYTHREAD03_H_ */
