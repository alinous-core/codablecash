/*
 * NopSmartcontractTransaction.cpp
 *
 *  Created on: 2023/12/04
 *      Author: iizuka
 */
#include "transaction/NopSmartcontractTransaction.h"

#include "bc_base/BalanceUnit.h"
#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

#include "base_timestamp/SystemTimestamp.h"

#include "crypto/Sha256.h"

#include "bc_trx/TransactionId.h"
#include "bc_trx/AbstractUtxo.h"


namespace codablecash {

uint64_t NopSmartcontractTransaction::serial = 1;

NopSmartcontractTransaction::NopSmartcontractTransaction(const NopSmartcontractTransaction &inst)
		: AbstractSmartcontractTransaction(inst){
	this->nonce = inst.nonce;
	this->utxoList = new ArrayList<AbstractUtxo>();

	int maxLoop = inst.utxoList->size();
	for(int i = 0; i != maxLoop; ++i){
		const AbstractUtxo* utxo = inst.utxoList->get(i);

		this->utxoList->addElement(dynamic_cast<AbstractUtxo*>(utxo->copyData()));
	}
}

NopSmartcontractTransaction::NopSmartcontractTransaction() : AbstractSmartcontractTransaction() {
	this->nonce = serial++;
	this->utxoList = new ArrayList<AbstractUtxo>();
}

NopSmartcontractTransaction::~NopSmartcontractTransaction() {
	this->utxoList->deleteElements();
	delete this->utxoList;
}

uint8_t NopSmartcontractTransaction::getType() const noexcept {
	return TRX_TYPE_SMARTCONTRACT_NOP;
}

int NopSmartcontractTransaction::binarySize() const {
	BinaryUtils::checkNotNull(this->timestamp);

	int total = sizeof(uint8_t);

	total += this->timestamp->binarySize();
	total += sizeof(this->nonce);

	int maxLoop = this->utxoList->size();
	total += sizeof(uint16_t);

	for(int i = 0; i != maxLoop; ++i){
		const AbstractUtxo* utxo = this->utxoList->get(i);

		total += utxo->binarySize();
	}

	return total;
}

void NopSmartcontractTransaction::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->timestamp);

	out->put(getType());

	this->timestamp->toBinary(out);
	out->putLong(this->nonce);

	int maxLoop = this->utxoList->size();
	out->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		const AbstractUtxo* utxo = this->utxoList->get(i);

		utxo->toBinary(out);
	}
}

void NopSmartcontractTransaction::fromBinary(ByteBuffer *in) {
	delete this->timestamp;
	this->timestamp = SystemTimestamp::fromBinary(in);
	this->nonce = in->getLong();

	int maxLoop = in->getShort();
	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxo* utxo = AbstractUtxo::createFromBinary(in); __STP(utxo);
		BinaryUtils::checkNotNull(utxo);

		this->utxoList->addElement(__STP_MV(utxo));
	}
}

void NopSmartcontractTransaction::build() {
	setUtxoNonce();

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

void NopSmartcontractTransaction::setUtxoNonce() noexcept {
	int capacity = sizeof(uint8_t) + this->timestamp->binarySize();

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(capacity, true); __STP(buff);
	buff->put(getType());
	this->timestamp->toBinary(buff);


	ByteBuffer* sha = Sha256::sha256(buff, true); __STP(sha);
	const uint8_t* data = sha->array();

	int maxLoop = this->utxoList->size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxo* utxo = this->utxoList->get(i);

		utxo->setNonce(data, i);

		utxo->build();
	}
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
	return this->utxoList->size();
}

AbstractUtxo* NopSmartcontractTransaction::getUtxo(int i) const noexcept {
	return this->utxoList->get(i);
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

bool NopSmartcontractTransaction::checkFilter(const ArrayList<BloomFilter1024> *filtersList) const {
	return true;
}

void NopSmartcontractTransaction::addutxo(const AbstractUtxo *utxo) noexcept {
	this->utxoList->addElement(dynamic_cast<AbstractUtxo*>(utxo->copyData()));
}

} /* namespace codablecash */
