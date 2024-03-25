/*
 * CoinbaseTransaction.cpp
 *
 *  Created on: 2023/05/15
 *      Author: iizuka
 */

#include "bc_block_body/CoinbaseTransaction.h"
#include "bc_block_body/Coinbase.h"

#include "bc_base/BalanceUnit.h"
#include "bc_base/BinaryUtils.h"


#include "base/StackRelease.h"

#include "crypto/Sha256.h"

#include "bc_trx/TransactionId.h"

#include "bc_trx/AbstractUtxo.h"
namespace codablecash {

CoinbaseTransaction::CoinbaseTransaction(const CoinbaseTransaction &inst) : AbstractBlockRewordTransaction(inst) {
	this->coinbase = inst.coinbase != nullptr ? dynamic_cast<Coinbase*>(inst.coinbase->copyData()) : nullptr;
}

CoinbaseTransaction::CoinbaseTransaction() : AbstractBlockRewordTransaction() {
	this->coinbase = nullptr;
}

CoinbaseTransaction::~CoinbaseTransaction() {
	delete this->coinbase;
}

int CoinbaseTransaction::binarySize() const {
	BinaryUtils::checkNotNull(this->coinbase);

	int total = __binarySize();
	total += this->coinbase->binarySize();

	return total;
}

void CoinbaseTransaction::toBinary(ByteBuffer *out) const {
	__toBinary(out);
	this->coinbase->toBinary(out);
}

void CoinbaseTransaction::fromBinary(ByteBuffer *in) {
	__fromBinary(in);

	AbstractUtxoReference* ref = AbstractUtxoReference::createFromBinary(in); __STP(ref);
	this->coinbase = dynamic_cast<Coinbase*>(ref);
	BinaryUtils::checkNotNull(this->coinbase);
	__STP_MV(ref);
}

IBlockObject* CoinbaseTransaction::copyData() const noexcept {
	return new CoinbaseTransaction(*this);
}

void CoinbaseTransaction::build() {
	setUtxoNonce();

	int capacity = binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(capacity, true); __STP(buff);

	toBinary(buff);
	buff->position(0);

	ByteBuffer* sha = Sha256::sha256(buff, true); __STP(sha);

	delete this->trxId;
	this->trxId = new TransactionId((const char*)sha->array(), sha->limit());
}

int CoinbaseTransaction::getUtxoSize() const noexcept {
	return AbstractBlockRewordTransaction::getUtxoSize();
}

AbstractUtxo* CoinbaseTransaction::getUtxo(int i) const noexcept {
	return AbstractBlockRewordTransaction::getUtxo(i);
}

int CoinbaseTransaction::getUtxoReferenceSize() const noexcept {
	return AbstractBlockRewordTransaction::getUtxoReferenceSize() + 1;
}

AbstractUtxoReference* CoinbaseTransaction::getUtxoReference(int i) const noexcept {
	assert(i == 0);
	return this->coinbase;
}

BalanceUnit CoinbaseTransaction::getFeeRate() const noexcept {
	return BalanceUnit(0L);
}

bool CoinbaseTransaction::validateOnAccept(MemPoolTransaction *memTrx, IStatusCacheContext *context) const {
	return true;
}

TrxValidationResult CoinbaseTransaction::validateFinal(const BlockHeader *header
		, MemPoolTransaction *memTrx, IStatusCacheContext *context) const {
	const BalanceUnit* unit = this->coinbase->getAmount();

	BalanceUnit total(0L);

	int maxLoop = getUtxoSize();
	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxo* utxo = getUtxo(i);
		BalanceUnit amount = utxo->getAmount();

		total += amount;
	}

	return total.compareTo(unit) == 0 ? TrxValidationResult::OK : TrxValidationResult::INVALID;
}

void CoinbaseTransaction::setCoinbase(const Coinbase *coinbase) noexcept {
	delete this->coinbase;
	this->coinbase = dynamic_cast<Coinbase*>(coinbase->copyData());
}

bool CoinbaseTransaction::coinbaseEquals(const CoinbaseTransaction *otherTrx) const noexcept {
	return this->coinbase->equals(otherTrx->coinbase);
}

} /* namespace codablecash */
