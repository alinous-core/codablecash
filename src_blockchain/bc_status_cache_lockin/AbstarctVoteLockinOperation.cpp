/*
 * AbstarctVoteLockinOperation.cpp
 *
 *  Created on: Oct 13, 2024
 *      Author: iizuka
 */

#include "bc_status_cache_lockin/AbstarctVoteLockinOperation.h"

#include "base_io/ByteBuffer.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_base/BinaryUtils.h"

#include "bc_trx/UtxoId.h"

namespace codablecash {

AbstarctVoteLockinOperation::AbstarctVoteLockinOperation(const AbstarctVoteLockinOperation &inst)
		: AbstractLockinOperation(inst) {
	this->nodeId = inst.nodeId != nullptr ? dynamic_cast<NodeIdentifier*>(inst.nodeId->copyData()) : nullptr;
	this->utxoId = inst.utxoId != nullptr ? dynamic_cast<UtxoId*>(inst.utxoId->copyData()) : nullptr;
}

AbstarctVoteLockinOperation::AbstarctVoteLockinOperation(uint16_t type)
		: AbstractLockinOperation(type) {
	this->nodeId = nullptr;
	this->utxoId = nullptr;
}

AbstarctVoteLockinOperation::~AbstarctVoteLockinOperation() {
	delete this->nodeId;
	delete this->utxoId;
}

int AbstarctVoteLockinOperation::binarySize() const {
	BinaryUtils::checkNotNull(this->nodeId);
	BinaryUtils::checkNotNull(this->utxoId);

	int total = sizeof(uint16_t);

	total += this->nodeId->binarySize();
	total += this->utxoId->binarySize();

	return total;
}

void AbstarctVoteLockinOperation::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->nodeId);
	BinaryUtils::checkNotNull(this->utxoId);

	out->putShort(this->type);
	this->nodeId->toBinary(out);
	this->utxoId->toBinary(out);
}

void AbstarctVoteLockinOperation::fromBinary(ByteBuffer *in) {
	this->nodeId = NodeIdentifier::fromBinary(in);
	this->utxoId = UtxoId::fromBinary(in);

	BinaryUtils::checkNotNull(this->nodeId);
	BinaryUtils::checkNotNull(this->utxoId);
}

bool AbstarctVoteLockinOperation::dataEquals(const AbstractLockinOperation *op) const noexcept {
	const AbstarctVoteLockinOperation* vop = dynamic_cast<const AbstarctVoteLockinOperation*>(op);
	assert(vop != nullptr);

	return this->nodeId->equals(vop->nodeId) && this->utxoId->equals(vop->utxoId);
}

void AbstarctVoteLockinOperation::setNodeId(const NodeIdentifier *nodeId) {
	delete this->nodeId;
	this->nodeId = dynamic_cast<NodeIdentifier*>(nodeId->copyData());
}

void AbstarctVoteLockinOperation::setUtxoId(const UtxoId *utxoId) {
	delete this->utxoId;
	this->utxoId = dynamic_cast<UtxoId*>(utxoId->copyData());
}

} /* namespace codablecash */
