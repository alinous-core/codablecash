/*
 * AbstractClientNotify.cpp
 *
 *  Created on: 2023/10/05
 *      Author: iizuka
 */

#include "bc_p2p_cmd_client_notify/AbstractClientNotifyCommand.h"

#include "bc_p2p_cmd/SignatureVerifivcationErrorException.h"

#include "bc/ExceptionThrower.h"

#include "pubsub/IPubsubCommandListner.h"
#include "pubsub/IPubsubCommandExecutor.h"

#include "bc_p2p_cmd_client_notify/ClientExecutor.h"

namespace codablecash {

AbstractClientNotifyCommand::AbstractClientNotifyCommand(const AbstractClientNotifyCommand &info)
		: AbstractBlockchainPubsubCommand(info) {
}

AbstractClientNotifyCommand::AbstractClientNotifyCommand(uint32_t type) : AbstractBlockchainPubsubCommand(type) {

}

AbstractClientNotifyCommand::~AbstractClientNotifyCommand() {

}

AbstractCommandResponse* AbstractClientNotifyCommand::execute(const PubSubId *pubsubId, IPubsubCommandListner *listner) const {
	ExceptionThrower<SignatureVerifivcationErrorException>::throwExceptionIfCondition(!verify(), L"Client command error.", __FILE__, __LINE__);

	listner->fireExecuteCommand(pubsubId, this);

	IPubsubCommandExecutor* exec = listner->getExecutor();
	ClientExecutor* clientExec = dynamic_cast<ClientExecutor*>(exec);

	return executeNotify(pubsubId, clientExec);
}

} /* namespace codablecash */
