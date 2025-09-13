/*
 * ListStakingNodeAccess.cpp
 *
 *  Created on: Aug 10, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_access/ListStakingNodeAccess.h"

#include "bc_p2p/ClientNodeHandshake.h"

#include "bc_p2p_cmd_client/ClientListStakingNodeIdsCommand.h"
#include "bc_p2p_cmd_client/ClientListStakingNodeIdsCommandResponse.h"

#include "base/StackRelease.h"

#include "pubsub/PubsubCommandException.h"

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "bc/ExceptionThrower.h"
#include "bc/ISystemLogger.h"

namespace codablecash {

ListStakingNodeAccess::ListStakingNodeAccess(const ClientListStakingNodeIdsCommand* command) {
	this->command = command;
	this->response = nullptr;
}

ListStakingNodeAccess::~ListStakingNodeAccess() {
	this->command = nullptr;
	delete this->response;
}

void ListStakingNodeAccess::access(ClientNodeHandshake *handshake, ISystemLogger *logger) {
	{
		AbstractCommandResponse* res = handshake->sendCommnad(this->command); __STP(res);

		{
			UnicodeString* msg = res->toString(); __STP(msg);
			logger->debugLog(ISystemLogger::DEBUG_NODE_TRANSFER_RESPONSE, msg, __FILE__, __LINE__);
		}

		ClientListStakingNodeIdsCommandResponse* stakingResponse = dynamic_cast<ClientListStakingNodeIdsCommandResponse*>(res);
		ExceptionThrower<PubsubCommandException>::throwExceptionIfCondition(stakingResponse == nullptr, L"Bad Command Response Type.", __FILE__, __LINE__);
		__STP_MV(res);

		delete this->response;
		this->response = stakingResponse;
	}
}

} /* namespace codablecash */
