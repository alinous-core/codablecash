/*
 * AbstractControlTransaction.cpp
 *
 *  Created on: 2023/04/15
 *      Author: iizuka
 */

#include "bc_trx/AbstractControlTransaction.h"
#include "bc_trx/AbstractUtxo.h"

#include "bc_trx_balance/InputUtxoCollection.h"
#include "bc_trx_balance/BalanceUtxo.h"
#include "bc_trx_balance/BalanceUtxoSign.h"

#include "bc_base/BinaryUtils.h"
#include "bc_base/BalanceUnit.h"

#include "base/StackRelease.h"

#include "bc_trx/TransactionId.h"

#include "musig/MuSig.h"

#include "ecda/Secp256k1CompressedPoint.h"

#include "bc_trx_balance/BalanceUtxoReference.h"

#include "crypto/Sha256.h"

#include "base_timestamp/SystemTimestamp.h"
namespace codablecash {

AbstractControlTransaction::AbstractControlTransaction(const AbstractControlTransaction &inst)
		: AbstractBlockchainTransaction(inst) {
	this->inputs = new InputUtxoCollection(*inst.inputs);

	int maxLoop = inst.list.size();
	for(int i = 0; i != maxLoop; ++i){
		BalanceUtxo* utxo = inst.list.get(i);
		addBalanceUtxo(utxo);
	}

	this->fee = inst.fee;
	this->signature = inst.signature != nullptr ? dynamic_cast<BalanceUtxoSign*>(inst.signature->copyData()) : nullptr;
}

AbstractControlTransaction::AbstractControlTransaction() : AbstractBlockchainTransaction() {
	this->inputs = new InputUtxoCollection();
	this->signature = nullptr;
}

AbstractControlTransaction::~AbstractControlTransaction() {
	delete this->inputs;
	this->list.deleteElements();
	delete this->signature;
}

void AbstractControlTransaction::addInputUtxoRef(const BalanceUtxoReference *ref) noexcept {
	this->inputs->addReference(ref);
}

void AbstractControlTransaction::addBalanceUtxo(const BalanceUtxo *utxo) noexcept {
	BalanceUtxo* obj = dynamic_cast<BalanceUtxo*>(utxo->copyData());
	this->list.addElement(obj);
}

int AbstractControlTransaction::binarySize() const {
	int total = __binarySize();

	total += sizeof(uint8_t);
	if(this->signature != nullptr){
		total += this->signature->binarySize();
	}

	return total;
}

int AbstractControlTransaction::__binarySize() const {
	BinaryUtils::checkNotNull(this->timestamp);

	int total = sizeof(uint8_t);

	total += this->timestamp->binarySize();

	total += this->inputs->binarySize();

	total += sizeof(uint16_t);
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		BalanceUtxo* utxo = this->list.get(i);
		total += utxo->binarySize();
	}

	total += this->fee.binarySize();

	return total;
}

void AbstractControlTransaction::toBinary(ByteBuffer *out) const {
	__toBinary(out);

	bool bl = this->signature != nullptr;
	out->put(bl ? 1 : 0);
	if(bl){
		this->signature->toBinary(out);
	}
}
void AbstractControlTransaction::__toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->timestamp);

	out->put(getType());

	this->timestamp->toBinary(out);

	this->inputs->toBinary(out);

	int maxLoop = this->list.size();
	out->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		BalanceUtxo* utxo = this->list.get(i);
		utxo->toBinary(out);
	}

	this->fee.toBinary(out);
}

void AbstractControlTransaction::fromBinary(ByteBuffer *in) {
	__fromBinary(in);

	uint8_t bl = in->get();
	if(bl > 0){
		this->signature = BalanceUtxoSign::fromBinary(in);
	}
}

void AbstractControlTransaction::__fromBinary(ByteBuffer *in) {
	delete this->timestamp;
	this->timestamp = SystemTimestamp::fromBinary(in);

	delete this->inputs;
	this->inputs = InputUtxoCollection::fromBinary(in);

	int maxLoop = in->getShort();
	BinaryUtils::checkUShortRange(maxLoop, 0, MAX_INPUT_ELEMENT);
	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxo* u = AbstractUtxo::createFromBinary(in); __STP(u);

		BinaryUtils::checkUint8Value(u->getType(), AbstractUtxo::TRX_UTXO_BALANCE);
		BalanceUtxo* utxo = dynamic_cast<BalanceUtxo*>(u);
		addBalanceUtxo(utxo);
	}

	{
		BalanceUnit* unit = BalanceUnit::fromBinary(in); __STP(unit);
		this->fee = *unit;
	}
}

void AbstractControlTransaction::setFeeAmount(const BalanceUnit *fee) {
	this->fee = *fee;

	BalanceUtxo* utxo = this->list.get(0);
	utxo->discountFee(*fee);
}

void AbstractControlTransaction::sign(IMuSigSignerProvidor *providor,	IUtxoFinder *finder) {
	assert(this->trxId != nullptr);

	int length = this->trxId->size();
	const char* data = this->trxId->toArray();

	MuSig sig = this->inputs->sign(providor, finder, data, length);
	const Secp256k1Point* R = sig.getR();
	Secp256k1CompressedPoint __R = Secp256k1CompressedPoint::compress(R);

	const BigInteger* s = sig.gets();

	delete this->signature;
	this->signature = new BalanceUtxoSign(&__R, s);
}

bool AbstractControlTransaction::verify() const {
	assert(this->signature != nullptr);
	assert(this->trxId != nullptr);

	const Secp256k1CompressedPoint* R = this->signature->getR();
	Secp256k1Point ptR = R->decompress();
	const BigInteger* s = this->signature->gets();

	MuSig sig(ptR, *s);

	const ArrayList<BalanceUtxoReference>* list = this->inputs->getList();
	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		BalanceUtxoReference* ref = list->get(i);

		const Secp256k1CompressedPoint* XiCompressed = ref->getXi();
		Secp256k1Point Xi = XiCompressed->decompress();

		sig.addXi(Xi);
	}

	int length = this->trxId->size();
	const char* data = this->trxId->toArray();

	bool bl = sig.verify(data, length);
	return bl;
}

void AbstractControlTransaction::setUtxoNonce() noexcept {
	int capacity = sizeof(uint8_t) + this->timestamp->binarySize() + this->inputs->binarySize();

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(capacity, true); __STP(buff);
	buff->put(getType());
	this->timestamp->toBinary(buff);
	this->inputs->toBinary(buff);

	__ASSERT_POS(buff);

	ByteBuffer* sha = Sha256::sha256(buff, true); __STP(sha);
	const uint8_t* data = sha->array();

	int maxLoop = getUtxoSize();
	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxo* utxo = getUtxo(i);

		utxo->setNonce(data, i);

		utxo->build();
	}
}


BalanceUnit AbstractControlTransaction::getFeeRate() const noexcept {
	uint64_t size = binarySize();
	uint64_t lfee = this->fee.getAmount();

	uint64_t rate = lfee /size;

	return BalanceUnit(rate);
}

int AbstractControlTransaction::getUtxoSize() const noexcept {
	return this->list.size();
}

AbstractUtxo* AbstractControlTransaction::getUtxo(int i) const noexcept {
	return this->list.get(i);
}

int AbstractControlTransaction::getUtxoReferenceSize() const noexcept {
	return this->inputs->getList()->size();
}

AbstractUtxoReference* AbstractControlTransaction::getUtxoReference(int i) const noexcept {
	return this->inputs->getList()->get(i);
}

BalanceUnit AbstractControlTransaction::getFee() const noexcept {
	return this->fee;
}

} /* namespace codablecash */
