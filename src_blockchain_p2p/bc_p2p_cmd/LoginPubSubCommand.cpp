/*
 * LoginCommand.cpp
 *
 *  Created on: 2023/04/05
 *      Author: iizuka
 */

#include "bc_p2p_cmd/LoginPubSubCommand.h"
#include "bc_p2p_cmd/SignatureErrorResponse.h"
#include "bc_p2p_cmd/P2pHandshakeAuthenticationException.h"

#include "pubsub/IPubsubCommandListner.h"
#include "pubsub/IPubsubCommandExecutor.h"

#include "pubsub_cmd/OkPubsubResponse.h"
#include "pubsub_cmd/ErrorPubsubResponse.h"

#include "base_io/ByteBuffer.h"

#include "bc_network/NodeIdentifier.h"

#include "base/Exception.h"

#include "bc/CodablecashNodeInstance.h"

#include "bc_p2p/BlochchainP2pManager.h"

#include "bc_p2p/StackHandshakeReleaser.h"

#include "bc_p2p_cmd_network/NodeNetworkInfo.h"

namespace codablecash {

LoginPubSubCommand::LoginPubSubCommand(const LoginPubSubCommand &inst)
		: AbstractBlockchainLoginCommand(inst) {
	this->nodeNetInfo = inst.nodeNetInfo != nullptr ? new NodeNetworkInfo(*inst.nodeNetInfo) : nullptr;
}

LoginPubSubCommand::LoginPubSubCommand(uint16_t zone, const UnicodeString* canonicalName)
	: AbstractBlockchainLoginCommand(AbstractPubSubCommand::TYPE_LOGIN, zone, canonicalName) {
	this->nodeNetInfo = nullptr;
}

LoginPubSubCommand::~LoginPubSubCommand() {
	delete this->nodeNetInfo;
}

int LoginPubSubCommand::binarySize() const {
	int total = AbstractBlockchainLoginCommand::binarySize();

	return total;
}

void LoginPubSubCommand::toBinary(ByteBuffer *buff) const {
	AbstractBlockchainLoginCommand::toBinary(buff);
}

void LoginPubSubCommand::fromBinary(ByteBuffer *buff) {
	AbstractBlockchainLoginCommand::fromBinary(buff);
}

AbstractCommandResponse* LoginPubSubCommand::execute(const PubSubId* pubsubId, IPubsubCommandListner *listner) const {
	try{
		checkSignature();
	}
	catch(Exception* e){
		delete e;
		return new SignatureErrorResponse();
	}

	{
		IPubsubCommandExecutor* executor = listner->getExecutor();
		CodablecashNodeInstance* inst = dynamic_cast<CodablecashNodeInstance*>(executor);
		BlochchainP2pManager* p2pMgr = inst->getBlochchainP2pManager();

		BlockchainNodeHandshake* node = p2pMgr->getNodeHandshakeByNodeId(this->nodeId);
		StackHandshakeReleaser __releaser(node);
		if(node != nullptr){
			ErrorPubsubResponse* res = new ErrorPubsubResponse();
			return res;
		}
	}

	listner->fireExecuteCommand(this);

	OkPubsubResponse* res = new OkPubsubResponse();
	return res;
}

IBlockObject* LoginPubSubCommand::copyData() const noexcept {
	return new LoginPubSubCommand(*this);
}

ByteBuffer* LoginPubSubCommand::getSignBinary() const {
	int cap = sizeof(this->zone);
	cap += this->nodeId->binarySize();

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(cap, true);
	buff->putShort(this->zone);
	this->nodeId->toBinary(buff);

	buff->position(0);
	return buff;
}

} /* namespace codablecash */
