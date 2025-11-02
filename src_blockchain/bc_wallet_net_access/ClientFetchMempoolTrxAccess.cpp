/*
 * ClientFetchMempoolTrxAccess.cpp
 *
 *  Created on: Aug 17, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_access/ClientFetchMempoolTrxAccess.h"

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "bc_p2p/ClientNodeHandshake.h"

#include "bc_p2p_cmd_client/ClientFetchMempoolTrxCommand.h"
#include "bc_p2p_cmd_client/ClientFetchMempoolTrxCommandResponse.h"

#include "base/StackRelease.h"

#include "bc/ISystemLogger.h"
#include "bc/ExceptionThrower.h"

#include "pubsub/PubsubCommandException.h"


namespace codablecash {

ClientFetchMempoolTrxAccess::ClientFetchMempoolTrxAccess(const ClientFetchMempoolTrxCommand* command) {
	this->command = command;
	this->response = nullptr;
}

ClientFetchMempoolTrxAccess::~ClientFetchMempoolTrxAccess() {
	this->command = nullptr;
	delete this->response;
}

void ClientFetchMempoolTrxAccess::access(ClientNodeHandshake *handshake, ISystemLogger *logger) {
	AbstractCommandResponse* res = handshake->sendCommnad(this->command); __STP(res);

	{
		UnicodeString* msg = res->toString(); __STP(msg);
		logger->debugLog(ISystemLogger::DEBUG_NODE_TRANSFER_RESPONSE, msg, __FILE__, __LINE__);
	}

	ClientFetchMempoolTrxCommandResponse* mempoolRes = dynamic_cast<ClientFetchMempoolTrxCommandResponse*>(res);
	ExceptionThrower<PubsubCommandException>::throwExceptionIfCondition(mempoolRes == nullptr, L"Bad Command Response Type.", __FILE__, __LINE__);
	__STP_MV(res);

	delete this->response;
	this->response = mempoolRes;
}

} /* namespace codablecash */
