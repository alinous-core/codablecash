/*
 * AbstractNodeCommand.cpp
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#include "bc_p2p_cmd_node/AbstractNodeCommand.h"

#include "bc_p2p_cmd/SignatureVerifivcationErrorException.h"

#include "bc_p2p/BlochchainP2pManager.h"
#include "bc_p2p/StackHandshakeReleaser.h"

#include "bc_p2p_processor/P2pRequestProcessor.h"

#include "pubsub/IPubsubCommandListner.h"

#include "bc/CodablecashNodeInstance.h"
#include "bc/ExceptionThrower.h"

#include "base_thread/StackUnlocker.h"
#include "base_thread/SynchronizedLock.h"

#include "pubsub_cmd/OkPubsubResponse.h"

#include "pubsub/PubsubCommandException.h"
namespace codablecash {

AbstractNodeCommand::AbstractNodeCommand(const AbstractNodeCommand &inst) : AbstractBlockchainPubsubCommand(inst) {
}


AbstractNodeCommand::AbstractNodeCommand(uint32_t type) : AbstractBlockchainPubsubCommand(type) {

}

AbstractNodeCommand::~AbstractNodeCommand() {

}
AbstractCommandResponse* AbstractNodeCommand::execute(const PubSubId *pubsubId,	IPubsubCommandListner *listner) const {
	IPubsubCommandExecutor* exec = listner->getExecutor();

	CodablecashNodeInstance* inst = dynamic_cast<CodablecashNodeInstance*>(exec);
	if(inst == nullptr){
		return new OkPubsubResponse();
	}

	// check if processor is suspended
	P2pRequestProcessor* requestProcessor = inst->getP2pRequestProcessor();
	SynchronizedLock* threadLock = requestProcessor->getSynchrinizedLock();
	{
		StackUnlocker unlocker(threadLock);
		if(requestProcessor->__isSuspended()){
			if(usePendingQueue()){
				requestProcessor->__putPendingCommand(this, pubsubId);
			}

			return new OkPubsubResponse();
		}
	}

	BlochchainP2pManager* p2pManager = inst->getBlochchainP2pManager();

	BlockchainNodeHandshake* nodeHandShake = p2pManager->getNodeHandshake(pubsubId);
	StackHandshakeReleaser __releaser(nodeHandShake);

	ExceptionThrower<SignatureVerifivcationErrorException>::throwExceptionIfCondition(nodeHandShake == nullptr, L"Node is not connected.", __FILE__, __LINE__);
	ExceptionThrower<SignatureVerifivcationErrorException>::throwExceptionIfCondition(!verify(), L"Node signature verification error.", __FILE__, __LINE__);

	return executeAsNode(nodeHandShake, inst, false);
}

bool AbstractNodeCommand::usePendingQueue() const noexcept {
	return true;
}

} /* namespace codablecash */
