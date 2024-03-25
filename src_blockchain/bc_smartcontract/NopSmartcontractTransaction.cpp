/*
 * NopSmartcontractTransaction.cpp
 *
 *  Created on: 2023/12/04
 *      Author: iizuka
 */

#include "bc_smartcontract/NopSmartcontractTransaction.h"

#include "bc_base/BalanceUnit.h"
#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

#include "base_timestamp/SystemTimestamp.h"

#include "crypto/Sha256.h"

#include "bc_trx/TransactionId.h"

namespace codablecash {

uint64_t NopSmartcontractTransaction::serial = 1;

NopSmartcontractTransaction::NopSmartcontractTransaction(const NopSmartcontractTransaction &inst)
		: AbstractSmartcontractTransaction(inst){
	this->nonce = inst.nonce;
}

NopSmartcontractTransaction::NopSmartcontractTransaction() : AbstractSmartcontractTransaction() {
	this->nonce = serial++;
}

NopSmartcontractTransaction::~NopSmartcontractTransaction() {

}

uint8_t NopSmartcontractTransaction::getType() const noexcept {
	return TRX_TYPE_SMARTCONTRACT_NOP;
}

int NopSmartcontractTransaction::binarySize() const {
	BinaryUtils::checkNotNull(this->timestamp);

	int total = sizeof(uint8_t);

	total += this->timestamp->binarySize();
	total += sizeof(this->nonce);

	return total;
}

void NopSmartcontractTransaction::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->timestamp);

	out->put(getType());

	this->timestamp->toBinary(out);
	out->putLong(this->nonce);
}

void NopSmartcontractTransaction::fromBinary(ByteBuffer *in) {
	delete this->timestamp;
	this->timestamp = SystemTimestamp::fromBinary(in);
	this->nonce = in->getLong();
}

void NopSmartcontractTransaction::build() {
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

IBlockObject* NopSmartcontractTransaction::copyData() const noexcept {
	return new NopSmartcontractTransaction(*this);
}

BalanceUnit NopSmartcontractTransaction::getFee() const noexcept {
	return BalanceUnit(0);
}

BalanceUnit NopSmartcontractTransaction::getFeeRate() const noexcept {
	return BalanceUnit(0);
}

int NopSmartcontractTransaction::getUtxoSize() const noexcept {
	return 0;
}

AbstractUtxo* NopSmartcontractTransaction::getUtxo(int i) const noexcept {
	return nullptr;
}

int NopSmartcontractTransaction::getUtxoReferenceSize() const noexcept {
	return 0;
}

AbstractUtxoReference* NopSmartcontractTransaction::getUtxoReference(int i) const noexcept {
	return nullptr;
}

bool NopSmartcontractTransaction::validateOnAccept(MemPoolTransaction *memTrx, IStatusCacheContext *context) const {
	return true;
}

TrxValidationResult NopSmartcontractTransaction::validateFinal(const BlockHeader *header, MemPoolTransaction *memTrx, IStatusCacheContext *context) const {
	return TrxValidationResult::OK;
}

} /* namespace codablecash */
