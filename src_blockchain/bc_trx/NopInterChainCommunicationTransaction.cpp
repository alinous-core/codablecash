/*
 * NopInterChainCommunicationTransaction.cpp
 *
 *  Created on: 2023/12/04
 *      Author: iizuka
 */

#include "bc_trx/NopInterChainCommunicationTransaction.h"

#include "bc_base/BalanceUnit.h"

#include "base/StackRelease.h"

#include "crypto/Sha256.h"

#include "bc_trx/TransactionId.h"

#include "base_timestamp/SystemTimestamp.h"

#include "bc_base/BinaryUtils.h"
namespace codablecash {

uint64_t NopInterChainCommunicationTransaction::serial = 1;

NopInterChainCommunicationTransaction::NopInterChainCommunicationTransaction(const NopInterChainCommunicationTransaction &inst)
		: AbstractInterChainCommunicationTansaction(inst) {
	this->nonce = inst.nonce;
}

NopInterChainCommunicationTransaction::NopInterChainCommunicationTransaction() : AbstractInterChainCommunicationTansaction() {
	this->nonce = serial++;
}

NopInterChainCommunicationTransaction::~NopInterChainCommunicationTransaction() {

}

uint8_t NopInterChainCommunicationTransaction::getType() const noexcept {
	return TRX_TYPE_ICC_NOP;
}

int NopInterChainCommunicationTransaction::binarySize() const {
	BinaryUtils::checkNotNull(this->timestamp);

	int total = sizeof(uint8_t);

	total += this->timestamp->binarySize();
	total += sizeof(this->nonce);

	return total;
}

void NopInterChainCommunicationTransaction::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->timestamp);

	out->put(getType());

	this->timestamp->toBinary(out);
	out->putLong(this->nonce);
}

void NopInterChainCommunicationTransaction::fromBinary(ByteBuffer *in) {
	delete this->timestamp;
	this->timestamp = SystemTimestamp::fromBinary(in);
	this->nonce = in->getLong();
}

void NopInterChainCommunicationTransaction::build() {
	BinaryUtils::checkNotNull(this->timestamp);

	int capacity = sizeof(uint8_t) + this->timestamp->binarySize() + sizeof(this->nonce);

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(capacity, true); __STP(buff);
	buff->put(getType());

	this->timestamp->toBinary(buff);
	buff->putLong(this->nonce);

	buff->position(0);

	ByteBuffer* sha = Sha256::sha256(buff, true); __STP(sha);

	delete this->trxId;
	this->trxId = new TransactionId((const char*)sha->array(), sha->limit());
}

IBlockObject* NopInterChainCommunicationTransaction::copyData() const noexcept {
	return new NopInterChainCommunicationTransaction(*this);
}

BalanceUnit NopInterChainCommunicationTransaction::getFee() const noexcept {
	return BalanceUnit(0);
}

BalanceUnit NopInterChainCommunicationTransaction::getFeeRate() const noexcept {
	return BalanceUnit(0);
}

int NopInterChainCommunicationTransaction::getUtxoSize() const noexcept {
	return 0;
}

AbstractUtxo* NopInterChainCommunicationTransaction::getUtxo(int i) const noexcept {
	return nullptr;
}

int NopInterChainCommunicationTransaction::getUtxoReferenceSize() const noexcept {
	return 0;
}

AbstractUtxoReference* NopInterChainCommunicationTransaction::getUtxoReference(int i) const noexcept {
	return nullptr;
}

bool NopInterChainCommunicationTransaction::validateOnAccept(MemPoolTransaction *memTrx, IStatusCacheContext *context) const {
	return true;
}

TrxValidationResult NopInterChainCommunicationTransaction::validateFinal(
		const BlockHeader *header, MemPoolTransaction *memTrx,
		IStatusCacheContext *context) const {
	return TrxValidationResult::OK;
}

} /* namespace codablecash */
