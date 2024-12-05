/*
 * AbstractClientRequestCommand.cpp
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#include "bc_p2p_cmd_client/AbstractClientRequestCommand.h"

#include "bc_p2p/BlochchainP2pManager.h"
#include "bc_p2p/ClientNodeHandshake.h"

#include "pubsub/IPubsubCommandListner.h"
#include "pubsub/IPubsubCommandExecutor.h"

#include "pubsub_cmd/OkPubsubResponse.h"
#include "pubsub_cmd/ErrorPubsubResponse.h"

#include "bc/CodablecashNodeInstance.h"
#include "bc/ExceptionThrower.h"

#include "bc_p2p_cmd/SignatureVerifivcationErrorException.h"

#include "bc_p2p_processor/P2pRequestProcessor.h"

#include "base_thread/SynchronizedLock.h"
#include "base_thread/StackUnlocker.h"

#include "bc_network/NodeIdentifier.h"


namespace codablecash {

AbstractClientRequestCommand::AbstractClientRequestCommand(const AbstractClientRequestCommand &inst) : AbstractBlockchainPubsubCommand(inst) {

}

AbstractClientRequestCommand::AbstractClientRequestCommand(uint32_t type) : AbstractBlockchainPubsubCommand(type) {

}

AbstractClientRequestCommand::~AbstractClientRequestCommand() {

}

AbstractCommandResponse* AbstractClientRequestCommand::execute(const PubSubId *pubsubId, IPubsubCommandListner *listner) const {
	IPubsubCommandExecutor* exec = listner->getExecutor();

	CodablecashNodeInstance* inst = dynamic_cast<CodablecashNodeInstance*>(exec);
	BlochchainP2pManager* p2pManager = inst->getBlochchainP2pManager();

	ClientNodeHandshake* clientHandshake = p2pManager->getClientHandshake(pubsubId);

	ExceptionThrower<SignatureVerifivcationErrorException>::throwExceptionIfCondition(clientHandshake == nullptr, L"Client is not registered.", __FILE__, __LINE__);
	ExceptionThrower<SignatureVerifivcationErrorException>::throwExceptionIfCondition(!verify(), L"Client command error.", __FILE__, __LINE__);

	const NodeIdentifier* clientNodeId = clientHandshake->getNodeId();
	ExceptionThrower<SignatureVerifivcationErrorException>::throwExceptionIfCondition(clientNodeId->compareTo(this->nodeId) != 0, L"Client is not registered.", __FILE__, __LINE__);

	// check if processor is suspended
	P2pRequestProcessor* requestProcessor = inst->getP2pRequestProcessor();
	if(requestProcessor != nullptr) {
		SynchronizedLock* threadLock = requestProcessor->getSynchrinizedLock();
		{
			StackUnlocker unlocker(threadLock, __FILE__, __LINE__);
			if(requestProcessor->__isSuspended()){
				// respond error
				ErrorPubsubResponse* res = new ErrorPubsubResponse();
				UnicodeString message(L"Node's processor is not ready.");

				return res;
			}
		}
	}

	return executeAsClient(clientHandshake, inst);
}

} /* namespace codablecash */
