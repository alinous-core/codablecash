/*
 * AbstractNodeNetworkNotifyCommand.cpp
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#include "bc_p2p_cmd_network/AbstractNodeNetworkNotifyCommand.h"

namespace codablecash {

AbstractNodeNetworkNotifyCommand::AbstractNodeNetworkNotifyCommand(uint32_t type) : AbstractBlockchainPubsubCommand(type) {

}

AbstractNodeNetworkNotifyCommand::~AbstractNodeNetworkNotifyCommand() {

}

} /* namespace codablecash */
