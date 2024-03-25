/*
 * NodeLiveCommand.cpp
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#include "bc_p2p_cmd_network/NodeLiveCommand.h"
#include "bc_p2p_cmd_network/NodeLiveData.h"

namespace codablecash {

NodeLiveCommand::NodeLiveCommand() : AbstractNodeNetworkNotifyCommand(TYPE_NETWORK_NODE_LIVE) {
	this->data = nullptr;
}

NodeLiveCommand::~NodeLiveCommand() {
	delete this->data;
}

} /* namespace codablecash */
