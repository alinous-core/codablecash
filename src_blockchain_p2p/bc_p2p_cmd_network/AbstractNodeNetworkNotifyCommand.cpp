/*
 * AbstractNodeNetworkNotifyCommand.cpp
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#include "bc_p2p_cmd_network/AbstractNodeNetworkNotifyCommand.h"

#include "pubsub/IPubsubCommandExecutor.h"
#include "pubsub/IPubsubCommandListner.h"
#include "pubsub/PubsubNetworkException.h"

#include "bc_p2p/BlochchainP2pManager.h"
#include "bc_p2p/StackHandshakeReleaser.h"

#include "bc/CodablecashNodeInstance.h"
#include "bc/ExceptionThrower.h"

#include "bc_p2p_cmd/SignatureVerifivcationErrorException.h"


namespace codablecash {

AbstractNodeNetworkNotifyCommand::AbstractNodeNetworkNotifyCommand(const AbstractNodeNetworkNotifyCommand &inst)
		: AbstractNodeCommand(inst) {
}

AbstractNodeNetworkNotifyCommand::AbstractNodeNetworkNotifyCommand(uint32_t type) : AbstractNodeCommand(type) {

}

AbstractNodeNetworkNotifyCommand::~AbstractNodeNetworkNotifyCommand() {

}

bool AbstractNodeNetworkNotifyCommand::usePendingQueue() const noexcept {
	return false;
}

} /* namespace codablecash */
