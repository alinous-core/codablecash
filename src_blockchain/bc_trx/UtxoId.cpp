/*
 * UtxoId.cpp
 *
 *  Created on: 2023/02/23
 *      Author: iizuka
 */

#include "bc_trx/UtxoId.h"

#include "base/StackRelease.h"

#include "numeric/BigInteger.h"


namespace codablecash {

UtxoId::UtxoId() {

}

UtxoId::UtxoId(const char *binary, int length) : Abstract32BytesId(binary, length) {

}

UtxoId::~UtxoId() {

}

IBlockObject* UtxoId::copyData() const noexcept {
	return new UtxoId(*this);
}

UtxoId* UtxoId::fromBinary(ByteBuffer *in) {
	UtxoId* utxoId = new UtxoId();

	int cap = in->getInt();

	uint8_t* buff = new uint8_t[cap];
	StackArrayRelease<uint8_t> __st_buff(buff);

	in->get(buff, cap);

	utxoId->id = ByteBuffer::wrapWithEndian(buff, cap, true);
	utxoId->id->position(0);

	return utxoId;
}

} /* namespace codablecash */
