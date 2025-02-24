/*
 * NodeShutdownCommand.cpp
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#include "bc_p2p_cmd_network/NodeShutdownCommand.h"

#include "pubsub/IPubsubCommandListner.h"
#include "pubsub/PubsubNetworkException.h"

#include "pubsub_cmd/OkPubsubResponse.h"

#include "bc/CodablecashNodeInstance.h"
#include "bc/ExceptionThrower.h"

#include "bc_p2p/BlochchainP2pManager.h"
#include "bc_p2p/StackHandshakeReleaser.h"

#include "bc_p2p_cmd/SignatureVerifivcationErrorException.h"

#include "bc_network/NodeIdentifier.h"
#include "bc_network/NodeIdentifierSource.h"

#include "bc_base/BinaryUtils.h"

#include "base_timestamp/SystemTimestamp.h"

#include "base/StackRelease.h"

#include "crypto/Schnorr.h"

#include "bc_p2p_processor/P2pRequestProcessor.h"

#include "bc_p2p_info/P2pDnsManager.h"

namespace codablecash {

NodeShutdownCommand::NodeShutdownCommand(const NodeShutdownCommand &inst) : AbstractNodeNetworkNotifyCommand(inst) {
	this->hops = inst.hops;
	this->targetNodeId = inst.targetNodeId != nullptr ? dynamic_cast<NodeIdentifier*>(inst.targetNodeId->copyData()) : nullptr;
	this->tm = inst.tm != nullptr ? dynamic_cast<SystemTimestamp*>(inst.tm->copyData()) : nullptr;
	this->tmSig = inst.tmSig != nullptr ? new SchnorrSignature(*inst.tmSig) : nullptr;
}

NodeShutdownCommand::NodeShutdownCommand() : AbstractNodeNetworkNotifyCommand(TYPE_NETWORK_NODE_SHUTDOWN) {
	this->hops = 0;
	this->targetNodeId = nullptr;
	this->tm = new SystemTimestamp();
	this->tmSig = nullptr;
}

NodeShutdownCommand::~NodeShutdownCommand() {
	delete this->targetNodeId;
	delete this->tm;
	delete this->tmSig;
}

IBlockObject* NodeShutdownCommand::copyData() const noexcept {
	return new NodeShutdownCommand(*this);
}

int NodeShutdownCommand::binarySize() const {
	BinaryUtils::checkNotNull(this->targetNodeId);
	BinaryUtils::checkNotNull(this->tm);
	BinaryUtils::checkNotNull(this->tmSig);

	int total = AbstractNodeNetworkNotifyCommand::binarySize();

	total += sizeof(this->hops);
	total += this->targetNodeId->binarySize();
	total += this->tm->binarySize();
	total += this->tmSig->binarySize();

	return total;
}

void NodeShutdownCommand::toBinary(ByteBuffer *buff) const {
	BinaryUtils::checkNotNull(this->targetNodeId);
	BinaryUtils::checkNotNull(this->tm);
	BinaryUtils::checkNotNull(this->tmSig);

	AbstractNodeNetworkNotifyCommand::toBinary(buff);

	buff->put(this->hops);
	this->targetNodeId->toBinary(buff);
	this->tm->toBinary(buff);
	this->tmSig->toBinary(buff);
}

void NodeShutdownCommand::fromBinary(ByteBuffer *buff) {
	AbstractNodeNetworkNotifyCommand::fromBinary(buff);

	this->hops = buff->get();
	this->targetNodeId = NodeIdentifier::fromBinary(buff);
	delete this->tm;
	this->tm = nullptr;
	this->tm = SystemTimestamp::fromBinary(buff);
	this->tmSig = SchnorrSignature::createFromBinary(buff);

	BinaryUtils::checkNotNull(this->targetNodeId);
	BinaryUtils::checkNotNull(this->tm);
	BinaryUtils::checkNotNull(this->tmSig);
}

ByteBuffer* NodeShutdownCommand::getSignBinary() const {
	BinaryUtils::checkNotNull(this->targetNodeId);
	BinaryUtils::checkNotNull(this->tm);
	BinaryUtils::checkNotNull(this->tmSig);

	int total = sizeof(this->hops);
	total += this->targetNodeId->binarySize();
	total += this->tm->binarySize();
	total += this->tmSig->binarySize();

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true);
	buff->put(this->hops);
	this->targetNodeId->toBinary(buff);
	this->tm->toBinary(buff);
	this->tmSig->toBinary(buff);

	return buff;
}

void NodeShutdownCommand::signTimestamp(const NodeIdentifierSource *source) {
	BinaryUtils::checkNotNull(this->tm);

	delete this->targetNodeId;
	delete this->tmSig;

	NodeIdentifier nId = source->toNodeIdentifier();
	this->targetNodeId = dynamic_cast<NodeIdentifier*>(nId.copyData());

	ByteBuffer* buff = getSignTimestampBinary(); __STP(buff);

	const BigInteger* publicKey = nId.getPublicKey();
	const BigInteger* secret = source->getSecretKey();

	this->tmSig = Schnorr::sign(*secret, *publicKey, buff->array(), buff->limit());
}

ByteBuffer* NodeShutdownCommand::getSignTimestampBinary() const {
	int total = this->targetNodeId->binarySize();
	total +=  this->tm->binarySize();

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true); __STP(buff);
	this->targetNodeId->toBinary(buff);
	this->tm->toBinary(buff);

	return __STP_MV(buff);
}

bool NodeShutdownCommand::verifyTimestamp() const {
	ByteBuffer* buff = getSignTimestampBinary(); __STP(buff);

	const BigInteger* publicKey = this->targetNodeId->getPublicKey();
	bool result = Schnorr::verify(*this->tmSig, *publicKey, buff->array(), buff->limit());

	return result;
}

AbstractCommandResponse* NodeShutdownCommand::executeAsNode(BlockchainNodeHandshake *nodeHandShake, CodablecashNodeInstance *inst, bool suspend) const {
	{
		bool vl = verifyTimestamp();
		ExceptionThrower<SignatureVerifivcationErrorException>::throwExceptionIfCondition(!vl, L"Node signature verification error.", __FILE__, __LINE__);
	}

	bool updated = updateShutdownNode(inst);

	if(!suspend && updated && this->hops < 5){
		const NodeIdentifierSource* source = inst->getNetworkKey();

		NodeShutdownCommand commnad(*this);
		commnad.incHpos();
		commnad.sign(source);

		// TODO Auto-generated constructor stub
		BlochchainP2pManager* p2pManager = inst->getBlochchainP2pManager();

	}

	return new OkPubsubResponse();
}

bool NodeShutdownCommand::updateShutdownNode(CodablecashNodeInstance *inst) const {
	P2pDnsManager* dnsManager = inst->getP2pDnsManager();
	bool removed = dnsManager->removeRecord(this->targetNodeId);

	{
		BlochchainP2pManager* p2pManager = inst->getBlochchainP2pManager();
		p2pManager->disconnect(this->targetNodeId);
	}

	return removed;
}


} /* namespace codablecash */
