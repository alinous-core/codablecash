/*
 * RevokeMissedTicket.cpp
 *
 *  Created on: 2023/05/17
 *      Author: iizuka
 */

#include "bc_finalizer_trx/RevokeMissedTicket.h"
#include "bc_finalizer_trx/TicketUtxoReference.h"

#include "bc_base/BinaryUtils.h"

#include "bc_trx/TransactionId.h"

#include "bc_trx_balance/BalanceUtxo.h"

#include "base/StackRelease.h"

#include "crypto/Sha256.h"

#include "base_timestamp/SystemTimestamp.h"

namespace codablecash {

RevokeMissedTicket::RevokeMissedTicket(const RevokeMissedTicket &inst) : AbstractRevokeTransaction(inst) {
	this->ticketUtxoRef = inst.ticketUtxoRef != nullptr ? dynamic_cast<TicketUtxoReference*>(inst.ticketUtxoRef->copyData()) : nullptr;
}

RevokeMissedTicket::RevokeMissedTicket() : AbstractRevokeTransaction() {
	this->ticketUtxoRef = nullptr;
}

RevokeMissedTicket::~RevokeMissedTicket() {
	delete this->ticketUtxoRef;
}

int RevokeMissedTicket::binarySize() const {
	BinaryUtils::checkNotNull(this->timestamp);
	BinaryUtils::checkNotNull(this->ticketUtxoRef);

	int total = sizeof(uint8_t);

	total += this->timestamp->binarySize();
	total += this->ticketUtxoRef->binarySize();

	int maxLoop = this->list.size();
	total += sizeof(uint8_t);

	for(int i = 0; i != maxLoop; ++i){
		BalanceUtxo* utxo = this->list.get(i);
		total += utxo->binarySize();
	}

	return total;
}

void RevokeMissedTicket::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->timestamp);
	BinaryUtils::checkNotNull(this->ticketUtxoRef);

	out->put(getType());

	this->timestamp->toBinary(out);
	this->ticketUtxoRef->toBinary(out);

	int maxLoop = this->list.size();
	out->put(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		BalanceUtxo* utxo = this->list.get(i);
		utxo->toBinary(out);
	}
}

void RevokeMissedTicket::fromBinary(ByteBuffer *in) {
	delete this->timestamp;
	this->timestamp = SystemTimestamp::fromBinary(in);

	AbstractUtxoReference* ref = AbstractUtxoReference::createFromBinary(in); __STP(ref);
	this->ticketUtxoRef = dynamic_cast<TicketUtxoReference*>(ref);
	BinaryUtils::checkNotNull(this->ticketUtxoRef);
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

IBlockObject* RevokeMissedTicket::copyData() const noexcept {
	return new RevokeMissedTicket(*this);
}

void RevokeMissedTicket::build() {
	setUtxoNonce();

	int capacity = __binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(capacity, true); __STP(buff);

	__toBinary(buff);
	buff->position(0);

	ByteBuffer* sha = Sha256::sha256(buff, true); __STP(sha);

	delete this->trxId;
	this->trxId = new TransactionId((const char*)sha->array(), sha->limit());
}

bool RevokeMissedTicket::validateOnAccept(MemPoolTransaction *memTrx,	IStatusCacheContext *context) const {
	UtxoValidationResult res = validateUtxos(memTrx, context, this->fee);

	return res != UtxoValidationResult::INVALID;
}

TrxValidationResult RevokeMissedTicket::validateFinal(const BlockHeader *header,
		MemPoolTransaction *memTrx, IStatusCacheContext *context) const {
	bool res = validateOnAccept(memTrx, context);

	UtxoValidationResult utxoRes = validateUtxos(memTrx, context, this->fee);

	return (res == true && utxoRes != UtxoValidationResult::INVALID)
			? TrxValidationResult::OK : TrxValidationResult::INVALID;
}

AbstractUtxoReference* RevokeMissedTicket::getUtxoReference(int i) const noexcept {
	return this->ticketUtxoRef;
}

int RevokeMissedTicket::getUtxoReferenceSize() const noexcept {
	return 1;
}

void RevokeMissedTicket::setTicketUtxoReference(const TicketUtxoReference *ref) {
	delete this->ticketUtxoRef;
	this->ticketUtxoRef = dynamic_cast<TicketUtxoReference*>(ref->copyData());
}

} /* namespace codablecash */
