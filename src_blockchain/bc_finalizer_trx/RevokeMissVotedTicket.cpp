/*
 * RevokeMissVotedTicket.cpp
 *
 *  Created on: 2023/05/17
 *      Author: iizuka
 */

#include "bc_finalizer_trx/RevokeMissVotedTicket.h"

#include "bc_finalizer_trx/TicketVotedUtxoReference.h"

#include "bc_base/BinaryUtils.h"

#include "bc_trx/TransactionId.h"

#include "bc_trx_balance/BalanceUtxo.h"

#include "base/StackRelease.h"

#include "crypto/Sha256.h"

#include "base_timestamp/SystemTimestamp.h"

namespace codablecash {

RevokeMissVotedTicket::RevokeMissVotedTicket(	const RevokeMissVotedTicket &inst)
			: AbstractRevokeTransaction(inst) {
	this->ticketVoteUtxoRef = inst.ticketVoteUtxoRef != nullptr ? dynamic_cast<TicketVotedUtxoReference*>(inst.ticketVoteUtxoRef->copyData()) : nullptr;
}

RevokeMissVotedTicket::RevokeMissVotedTicket() : AbstractRevokeTransaction() {
	this->ticketVoteUtxoRef = nullptr;
}

RevokeMissVotedTicket::~RevokeMissVotedTicket() {
	delete this->ticketVoteUtxoRef;
}

int RevokeMissVotedTicket::binarySize() const {
	BinaryUtils::checkNotNull(this->timestamp);
	BinaryUtils::checkNotNull(this->ticketVoteUtxoRef);

	int total = sizeof(uint8_t);

	total += this->timestamp->binarySize();
	total += this->ticketVoteUtxoRef->binarySize();

	int maxLoop = this->list.size();
	total += sizeof(uint8_t);

	for(int i = 0; i != maxLoop; ++i){
		BalanceUtxo* utxo = this->list.get(i);
		total += utxo->binarySize();
	}

	return total;
}

void RevokeMissVotedTicket::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->timestamp);
	BinaryUtils::checkNotNull(this->ticketVoteUtxoRef);

	out->put(getType());

	this->timestamp->toBinary(out);
	this->ticketVoteUtxoRef->toBinary(out);

	int maxLoop = this->list.size();
	out->put(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		BalanceUtxo* utxo = this->list.get(i);
		utxo->toBinary(out);
	}
}

void RevokeMissVotedTicket::fromBinary(ByteBuffer *in) {
	delete this->timestamp;
	this->timestamp = SystemTimestamp::fromBinary(in);

	AbstractUtxoReference* ref = AbstractUtxoReference::createFromBinary(in); __STP(ref);
	this->ticketVoteUtxoRef = dynamic_cast<TicketVotedUtxoReference*>(ref);
	BinaryUtils::checkNotNull(this->ticketVoteUtxoRef);
	__STP_MV(ref);

	int maxLoop = in->get();
	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxo* u = AbstractUtxo::createFromBinary(in); __STP(u);
		BalanceUtxo* utxo = dynamic_cast<BalanceUtxo*>(u);
		BinaryUtils::checkNotNull(utxo);
		__STP_MV(u);

		this->list.addElement(utxo);
	}
}

IBlockObject* RevokeMissVotedTicket::copyData() const noexcept {
	return new RevokeMissVotedTicket(*this);
}

void RevokeMissVotedTicket::build() {
	setUtxoNonce();

	int capacity = __binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(capacity, true); __STP(buff);

	__toBinary(buff);

	buff->position(0);

	ByteBuffer* sha = Sha256::sha256(buff, true); __STP(sha);

	delete this->trxId;
	this->trxId = new TransactionId((const char*)sha->array(), sha->limit());
}

bool RevokeMissVotedTicket::validateOnAccept(MemPoolTransaction *memTrx,	IStatusCacheContext *context) const {
	UtxoValidationResult res = validateUtxos(memTrx, context, this->fee);

	return res != UtxoValidationResult::INVALID;
}

TrxValidationResult RevokeMissVotedTicket::validateFinal(const BlockHeader *header, MemPoolTransaction *memTrx,
		IStatusCacheContext *context) const {
	bool res = validateOnAccept(memTrx, context);

	UtxoValidationResult utxoRes = validateUtxos(memTrx, context, this->fee);

	return (res == true && utxoRes != UtxoValidationResult::INVALID)
			? TrxValidationResult::OK : TrxValidationResult::INVALID;
}

AbstractUtxoReference* RevokeMissVotedTicket::getUtxoReference(int i) const noexcept {
	assert(i == 0);
	return this->ticketVoteUtxoRef;
}

int RevokeMissVotedTicket::getUtxoReferenceSize() const noexcept {
	return 1;
}

void RevokeMissVotedTicket::setTicketVotedUtxoReference(const TicketVotedUtxoReference *ref) noexcept {
	this->ticketVoteUtxoRef = dynamic_cast<TicketVotedUtxoReference*>(ref->copyData());
}

} /* namespace codablecash */
