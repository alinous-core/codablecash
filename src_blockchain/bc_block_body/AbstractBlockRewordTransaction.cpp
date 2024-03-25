/*
 * AbstractBlockRewordTransaction.cpp
 *
 *  Created on: 2023/05/15
 *      Author: iizuka
 */

#include "bc_block_body/AbstractBlockRewordTransaction.h"

#include "bc_trx_balance/InputUtxoCollection.h"

#include "bc_trx/AbstractUtxo.h"
#include "bc_trx/AbstractUtxoReference.h"

#include "base/StackRelease.h"

#include "crypto/Sha256.h"

#include "bc_base/BinaryUtils.h"
#include "bc_base/BalanceUnit.h"

#include "base_timestamp/SystemTimestamp.h"


namespace codablecash {

AbstractBlockRewordTransaction::AbstractBlockRewordTransaction(const AbstractBlockRewordTransaction &inst)
		: AbstractBalanceTransaction(inst) {
	this->height = inst.height;

	{
		int maxLoop = inst.inputlist.size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractUtxoReference* ref = inst.inputlist.get(i);
			addInputUtxoRef(ref);
		}
	}

	{
		int maxLoop = inst.list.size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractUtxo* utxo = inst.list.get(i);
			addUtxo(utxo);
		}
	}
}

AbstractBlockRewordTransaction::AbstractBlockRewordTransaction() : AbstractBalanceTransaction() {
	this->height = 0;
}

AbstractBlockRewordTransaction::~AbstractBlockRewordTransaction() {
	this->inputlist.deleteElements();
	this->list.deleteElements();
}

void AbstractBlockRewordTransaction::addUtxo(const AbstractUtxo *utxo) noexcept {
	this->list.addElement(dynamic_cast<AbstractUtxo*>(utxo->copyData()));
}

int AbstractBlockRewordTransaction::getUtxoSize() const noexcept {
	return this->list.size();
}

AbstractUtxo* AbstractBlockRewordTransaction::getUtxo(int i) const noexcept {
	return this->list.get(i);
}

int AbstractBlockRewordTransaction::getUtxoReferenceSize() const noexcept {
	return this->inputlist.size();
}

void AbstractBlockRewordTransaction::addInputUtxoRef(const AbstractUtxoReference *ref) noexcept {
	this->inputlist.addElement(dynamic_cast<AbstractUtxoReference*>(ref->copyData()));
}

AbstractUtxoReference* AbstractBlockRewordTransaction::getUtxoReference(int i) const noexcept {
	return this->inputlist.get(i);
}

void AbstractBlockRewordTransaction::setUtxoNonce() noexcept {
	assert(this->height != 0L);
	BinaryUtils::checkNotNull(this->timestamp);

	int capacity = sizeof(uint8_t) + this->timestamp->binarySize() + sizeof(this->height);
	{
		int maxLoop = getUtxoReferenceSize();
		for(int i = 0; i != maxLoop; ++i){
			AbstractUtxoReference* ref = getUtxoReference(i);

			capacity += ref->binarySize();
		}
	}

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(capacity, true); __STP(buff);
	buff->put(getType());
	this->timestamp->toBinary(buff);
	buff->putLong(this->height);
	{
		int maxLoop = getUtxoReferenceSize();
		for(int i = 0; i != maxLoop; ++i){
			AbstractUtxoReference* ref = getUtxoReference(i);

			ref->toBinary(buff);
		}
	}

	ByteBuffer* sha = Sha256::sha256(buff, true); __STP(sha);
	const uint8_t* data = sha->array();

	int maxLoop = getUtxoSize();
	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxo* utxo = getUtxo(i);

		utxo->setNonce(data, i);

		utxo->build();
	}
}

int AbstractBlockRewordTransaction::__binarySize() const {
	BinaryUtils::checkNotNull(this->timestamp);

	int total = sizeof(uint8_t);
	total += this->timestamp->binarySize();
	total += sizeof(this->height);

	{
		int maxLoop = this->inputlist.size();
		total += sizeof(uint8_t);

		for(int i = 0; i != maxLoop; ++i){
			AbstractUtxoReference* ref = this->inputlist.get(i);
			total += ref->binarySize();
		}
	}

	{
		int maxLoop = this->list.size();
		total += sizeof(uint8_t);

		for(int i = 0; i != maxLoop; ++i){
			AbstractUtxo* utxo = this->list.get(i);
			total += utxo->binarySize();
		}
	}

	return total;
}

void AbstractBlockRewordTransaction::__toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->timestamp);

	out->put(getType());
	this->timestamp->toBinary(out);
	out->putLong(this->height);

	{
		int maxLoop = this->inputlist.size();
		out->put(maxLoop);

		for(int i = 0; i != maxLoop; ++i){
			AbstractUtxoReference* ref = this->inputlist.get(i);
			ref->toBinary(out);
		}
	}

	{
		int maxLoop = this->list.size();
		out->put(maxLoop);

		for(int i = 0; i != maxLoop; ++i){
			AbstractUtxo* utxo = this->list.get(i);
			utxo->toBinary(out);
		}
	}
}

BalanceUnit AbstractBlockRewordTransaction::getFee() const noexcept {
	return BalanceUnit(0L);
}

void AbstractBlockRewordTransaction::__fromBinary(ByteBuffer *in) {
	delete this->timestamp;
	this->timestamp = SystemTimestamp::fromBinary(in);
	this->height = in->getLong();

	{
		int maxLoop = in->get();
		for(int i = 0; i != maxLoop; ++i){
			AbstractUtxoReference* ref = AbstractUtxoReference::createFromBinary(in); __STP(ref);
			BinaryUtils::checkNotNull(ref);

			this->inputlist.addElement(__STP_MV(ref));
		}
	}

	{
		int maxLoop = in->get();
		for(int i = 0; i != maxLoop; ++i){
			AbstractUtxo* utxo = AbstractUtxo::createFromBinary(in); __STP(utxo);
			BinaryUtils::checkNotNull(utxo);

			this->list.addElement(__STP_MV(utxo));
		}
	}

}

} /* namespace codablecash */
