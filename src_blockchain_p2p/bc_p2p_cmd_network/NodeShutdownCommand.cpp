/*
 * NodeShutdownCommand.cpp
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#include "bc_p2p_cmd_network/NodeShutdownCommand.h"

namespace codablecash {

NodeShutdownCommand::NodeShutdownCommand()  : AbstractNodeNetworkNotifyCommand(TYPE_NETWORK_NODE_SHUTDOWN) {
	// TODO Auto-generated constructor stub

}

NodeShutdownCommand::~NodeShutdownCommand() {

}

} /* namespace codablecash */
