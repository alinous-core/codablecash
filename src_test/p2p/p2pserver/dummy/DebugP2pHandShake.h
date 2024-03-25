/*
 * DebugP2pHandShake.h
 *
 *  Created on: 2023/01/16
 *      Author: iizuka
 */

#ifndef P2P_P2PSERVER_DUMMY_DEBUGP2PHANDSHAKE_H_
#define P2P_P2PSERVER_DUMMY_DEBUGP2PHANDSHAKE_H_

#include "pubsub/P2pHandshake.h"

namespace codablecash {

class DebugP2pHandShake : public P2pHandshake {
public:
	DebugP2pHandShake(const PubSubId* psId, ISystemLogger* logger);
	virtual ~DebugP2pHandShake();

#ifdef __DEBUG__
	void __test_connectPublisher(IClientSocket* con){
		connectPublisher(con);
	}

	void __test_connectSubscriber(IClientSocket* con){
		connectSubscriber(con, this, true);
	}

	void __test_checkConnection(IClientSocket *con) {
		checkConnection(con);
	}
#endif
};

} /* namespace codablecash */

#endif /* P2P_P2PSERVER_DUMMY_DEBUGP2PHANDSHAKE_H_ */
