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

#include "bc_p2p_processor/P2pRequestProcessor.h"
#include "bc_p2p_processor/NetworkInfoProcessor.h"


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

	total += sizeof(uint8_t);
	if(this->nodeNetInfo != nullptr){
		total += this->nodeNetInfo->binarySize();
	}

	return total;
}

void LoginPubSubCommand::toBinary(ByteBuffer *buff) const {
	AbstractBlockchainLoginCommand::toBinary(buff);

	uint8_t bl = this->nodeNetInfo != nullptr ? 1 : 0;
	buff->put(bl);

	if(bl > 0){
		this->nodeNetInfo->toBinary(buff);
	}
}

void LoginPubSubCommand::fromBinary(ByteBuffer *buff) {
	AbstractBlockchainLoginCommand::fromBinary(buff);

	uint8_t bl = buff->get();
	if(bl > 0){
		delete this->nodeNetInfo;
		this->nodeNetInfo = NodeNetworkInfo::fromBinary(buff);
	}
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

	// check & register dns
	if(this->nodeNetInfo){
		IPubsubCommandExecutor* executor = listner->getExecutor();
		CodablecashNodeInstance* inst = dynamic_cast<CodablecashNodeInstance*>(executor);
		P2pRequestProcessor* processer = inst->getP2pRequestProcessor();

		NetworkInfoProcessor* netDnsProcessor = processer->getNetworkInfoProcessor();
		netDnsProcessor->requestDnsCheck(this->nodeNetInfo);
	}

	OkPubsubResponse* res = new OkPubsubResponse();
	return res;
}

IBlockObject* LoginPubSubCommand::copyData() const noexcept {
	return new LoginPubSubCommand(*this);
}

ByteBuffer* LoginPubSubCommand::getSignBinary() const {
	int cap = sizeof(this->zone);
	cap += this->nodeId->binarySize();

	if(this->nodeNetInfo != nullptr){
		cap += this->nodeNetInfo->binarySize();
	}

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(cap, true);
	buff->putShort(this->zone);
	this->nodeId->toBinary(buff);

	if(this->nodeNetInfo != nullptr){
		this->nodeNetInfo->toBinary(buff);
	}

	buff->position(0);
	return buff;
}

void LoginPubSubCommand::setNodeNetworkInfo(const NodeNetworkInfo *nodeNetInfo) {
	delete this->nodeNetInfo;
	this->nodeNetInfo = new NodeNetworkInfo(*nodeNetInfo);
}

} /* namespace codablecash */
