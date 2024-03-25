/*
 * AbstractRevokeTransaction.cpp
 *
 *  Created on: 2023/05/17
 *      Author: iizuka
 */

#include "bc_finalizer_trx/AbstractRevokeTransaction.h"

#include "base/StackRelease.h"

#include "crypto/Sha256.h"

#include "base_io/ByteBuffer.h"

#include "bc_trx/AbstractUtxo.h"
#include "bc_trx/AbstractUtxoReference.h"

#include "bc_base/BinaryUtils.h"

#include "base_timestamp/SystemTimestamp.h"

namespace codablecash {

AbstractRevokeTransaction::AbstractRevokeTransaction(const AbstractRevokeTransaction &inst)
		: AbstractFinalizerTransaction(inst) {
}

AbstractRevokeTransaction::AbstractRevokeTransaction() : AbstractFinalizerTransaction() {

}

AbstractRevokeTransaction::~AbstractRevokeTransaction() {

}

void AbstractRevokeTransaction::setUtxoNonce() noexcept {
	int capacity = __binarySize();

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(capacity, true); __STP(buff);
	__toBinary(buff);
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

int AbstractRevokeTransaction::__binarySize() const {
	BinaryUtils::checkNotNull(this->timestamp);

	int capacity = sizeof(uint8_t) + this->timestamp->binarySize();

	int maxLoop = getUtxoReferenceSize();
	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxoReference* ref = getUtxoReference(i);
		capacity += ref->binarySize();
	}

	return capacity;
}

void AbstractRevokeTransaction::__toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->timestamp);

	out->put(getType());
	this->timestamp->toBinary(out);

	int maxLoop = getUtxoReferenceSize();
	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxoReference* ref = getUtxoReference(i);
		ref->toBinary(out);
	}
}

} /* namespace codablecash */
