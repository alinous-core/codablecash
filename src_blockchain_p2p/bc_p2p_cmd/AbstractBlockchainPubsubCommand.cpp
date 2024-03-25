/*
 * AbstractBlockchainPubsubCommand.cpp
 *
 *  Created on: 2023/04/05
 *      Author: iizuka
 */

#include "bc_p2p_cmd/AbstractBlockchainPubsubCommand.h"
#include "bc_p2p_cmd/P2pHandshakeAuthenticationException.h"

#include "bc_network/NodeIdentifier.h"
#include "bc_network/NodeIdentifierSource.h"

#include "crypto/SchnorrSignature.h"

#include "base/StackRelease.h"

#include "crypto/Schnorr.h"

#include "bc_base/BinaryUtils.h"

namespace codablecash {

AbstractBlockchainPubsubCommand::AbstractBlockchainPubsubCommand(const AbstractBlockchainPubsubCommand &inst)
		: AbstractPubSubCommand(inst) {
	this->nodeId = inst.nodeId != nullptr ? dynamic_cast<NodeIdentifier*>(inst.nodeId->copyData()) : nullptr;
	this->sig = inst.sig != nullptr ? new SchnorrSignature(*inst.sig) : nullptr;
}

AbstractBlockchainPubsubCommand::AbstractBlockchainPubsubCommand(uint32_t type) : AbstractPubSubCommand(type) {
	this->sig = nullptr;
	this->nodeId = nullptr;
}

AbstractBlockchainPubsubCommand::~AbstractBlockchainPubsubCommand() {
	delete this->sig;
	delete this->nodeId;
}

void AbstractBlockchainPubsubCommand::sign(const NodeIdentifierSource *source) {
	delete this->nodeId;
	delete this->sig;

	NodeIdentifier nId = source->toNodeIdentifier();
	this->nodeId = dynamic_cast<NodeIdentifier*>(nId.copyData());

	ByteBuffer* buff = getSignBinary(); __STP(buff);

	const BigInteger* publicKey = nId.getPublicKey();
	const BigInteger* secret = source->getSecretKey();
	this->sig = Schnorr::sign(*secret, *publicKey, buff->array(), buff->limit());
}

bool AbstractBlockchainPubsubCommand::verify() const {
	ByteBuffer* buff = getSignBinary(); __STP(buff);

	const BigInteger* publicKey = this->nodeId->getPublicKey();
	bool result = Schnorr::verify(*sig, *publicKey, buff->array(), buff->limit());

	return result;
}

int AbstractBlockchainPubsubCommand::binarySize() const {
	int total = sizeof(this->type);

	total += sigBinarySize();

	return total;
}

void AbstractBlockchainPubsubCommand::toBinary(ByteBuffer *buff) const {
	buff->putInt(this->type);

	toSigBinary(buff);
}

void AbstractBlockchainPubsubCommand::fromBinary(ByteBuffer *buff) {
	fromSigBinary(buff);
}

int AbstractBlockchainPubsubCommand::sigBinarySize() const {
	BinaryUtils::checkNotNull(this->nodeId);
	BinaryUtils::checkNotNull(this->sig);

	int total = this->nodeId->binarySize();
	total += this->sig->binarySize();

	return total;
}

void AbstractBlockchainPubsubCommand::toSigBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->nodeId);
	BinaryUtils::checkNotNull(this->sig);

	this->nodeId->toBinary(out);
	this->sig->toBinary(out);
}

void AbstractBlockchainPubsubCommand::setNodeIdentifier(const NodeIdentifier *nodeId) noexcept {
	delete this->nodeId;
	this->nodeId = new NodeIdentifier(*nodeId);
}

void AbstractBlockchainPubsubCommand::fromSigBinary(ByteBuffer *in) {
	this->nodeId = NodeIdentifier::fromBinary(in);
	this->sig = SchnorrSignature::createFromBinary(in);
}

void AbstractBlockchainPubsubCommand::checkSignature() const {
	if(!verify()){
		throw new P2pHandshakeAuthenticationException(__FILE__, __LINE__);
	}
}

} /* namespace codablecash */
