/*
 * GenesisTransaction.cpp
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#include "bc_trx_genesis/GenesisTransaction.h"

#include "bc_trx_balance/BalanceUtxo.h"

#include "base/StackRelease.h"

#include "base_timestamp/SystemTimestamp.h"

#include "crypto/Sha256.h"

#include "bc_trx/TransactionId.h"

#include "bc_base/BinaryUtils.h"
namespace codablecash {

GenesisTransaction::GenesisTransaction(const GenesisTransaction &inst)
		: AbstractBalanceTransaction(inst) {

	int maxLoop = inst.list.size();
	for(int i = 0; i != maxLoop; ++i){
		BalanceUtxo* utxo = inst.list.get(i);
		addBalanceUtxo(utxo);
	}
}

GenesisTransaction::GenesisTransaction() {

}

GenesisTransaction::~GenesisTransaction() {
	this->list.deleteElements();
}

void GenesisTransaction::addBalanceUtxo(const BalanceUtxo *utxo) noexcept {
	BalanceUtxo* obj = dynamic_cast<BalanceUtxo*>(utxo->copyData());
	this->list.addElement(obj);
}

int GenesisTransaction::binarySize() const {
	BinaryUtils::checkNotNull(this->timestamp);

	int total = sizeof(uint8_t);
	total += this->timestamp->binarySize();

	total += sizeof(uint16_t);
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		BalanceUtxo* utxo = this->list.get(i);

		total += utxo->binarySize();
	}

	return total;
}

void GenesisTransaction::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->timestamp);

	out->put(getType());
	this->timestamp->toBinary(out);

	int maxLoop = this->list.size();
	out->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		BalanceUtxo* utxo = this->list.get(i);
		utxo->toBinary(out);
	}
}

void GenesisTransaction::fromBinary(ByteBuffer *in) {
	delete this->timestamp;
	this->timestamp = SystemTimestamp::fromBinary(in);

	int maxLoop = in->getShort();
	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxo* utxo = AbstractUtxo::createFromBinary(in); __STP(utxo);

		addBalanceUtxo(dynamic_cast<BalanceUtxo*>(utxo));
	}
}

BalanceUnit GenesisTransaction::getTotalBalance() const noexcept {
	BalanceUnit unit(0L);

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		BalanceUtxo* utxo = this->list.get(i);

		const BalanceUnit& u = utxo->getAmount();
		unit = unit + u;
	}

	return unit;
}

IBlockObject* GenesisTransaction::copyData() const noexcept {
	GenesisTransaction* trx = new GenesisTransaction(*this);
	trx->build();

	return trx;
}

void GenesisTransaction::build() {
	setUtxoNonce();

	int cap = binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(cap, true); __STP(buff);

	toBinary(buff);
	buff->position(0);

	ByteBuffer* sha = Sha256::sha256(buff, true); __STP(sha);

	delete this->trxId;
	this->trxId = new TransactionId((const char*)sha->array(), sha->limit());
}

void GenesisTransaction::setUtxoNonce() noexcept {
	BinaryUtils::checkNotNull(this->timestamp);

	int capacity = sizeof(uint8_t) + this->timestamp->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(capacity, true); __STP(buff);
	buff->put(getType());
	this->timestamp->toBinary(buff);

	ByteBuffer* sha = Sha256::sha256(buff, true); __STP(sha);
	const uint8_t* data = sha->array();

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		BalanceUtxo* utxo = this->list.get(i);

		utxo->setNonce(data, i);

		utxo->build();
	}
}

BalanceUnit GenesisTransaction::getFeeRate() const noexcept {
	return BalanceUnit(0L);
}

BalanceUnit GenesisTransaction::getFee() const noexcept {
	return BalanceUnit(0L);
}

int GenesisTransaction::getUtxoReferenceSize() const noexcept {
	return 0;
}

AbstractUtxoReference* GenesisTransaction::getUtxoReference(int i) const noexcept {
	return nullptr;
}

bool GenesisTransaction::validateOnAccept(MemPoolTransaction *memTrx, IStatusCacheContext *context) const {
	return true;
}

TrxValidationResult GenesisTransaction::validateFinal(
		const BlockHeader *header, MemPoolTransaction *memTrx, IStatusCacheContext *context) const {
	return TrxValidationResult::OK;
}

} /* namespace codablecash */
