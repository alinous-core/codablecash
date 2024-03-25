/*
 * ClientConnectUtils.cpp
 *
 *  Created on: 2023/09/28
 *      Author: iizuka
 */

#include "ClientConnectUtils.h"

#include "base/StackRelease.h"

#include "ipconnect/IpV6ClientConnection.h"

#include "pubsub/PubSubId.h"

#include "p2pserver/ConnectPublisherCommand.h"

#include "bc_p2p_client/P2pClient.h"

namespace codablecash {

P2pClient* ClientConnectUtils::connect(const UnicodeString *host, int port, uint16_t zone, ISystemLogger *logger) {
	P2pClient* client = new P2pClient(zone, logger); __STP(client);
	client->connectIpV6(host, port);

	return __STP_MV(client);
}

} /* namespace codablecash */
