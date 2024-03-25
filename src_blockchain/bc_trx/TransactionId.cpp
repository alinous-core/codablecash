/*
 * TransactionId.cpp
 *
 *  Created on: 2019/01/05
 *      Author: iizuka
 */

#include "bc_trx/TransactionId.h"

#include "base_io/ByteBuffer.h"
#include "base/StackRelease.h"

namespace codablecash {

TransactionId::TransactionId() : Abstract32BytesId() {

}

TransactionId::TransactionId(const TransactionId& inst) : Abstract32BytesId(inst) {

}

TransactionId::TransactionId(const char* binary, int length) : Abstract32BytesId(binary, length) {

}

TransactionId::~TransactionId() {

}

int TransactionId::binarySize() const {
	return Abstract32BytesId::binarySize();
}

void TransactionId::toBinary(ByteBuffer *out) const {
	Abstract32BytesId::toBinary(out);
}

TransactionId* TransactionId::fromBinary(ByteBuffer* in) {
	TransactionId* trxId = new TransactionId();

	int cap = in->getInt();

	uint8_t* buff = new uint8_t[cap];
	StackArrayRelease<uint8_t> __st_buff(buff);

	in->get(buff, cap);

	trxId->id = ByteBuffer::wrapWithEndian(buff, cap, true);
	trxId->id->position(0);

	return trxId;
}

IBlockObject* TransactionId::copyData() const noexcept {
	return new TransactionId(*this);
}

int TransactionId::ValueCompare::operator ()(
		const TransactionId *const a,
		const TransactionId *const b) const noexcept {
	return a->compareTo(b);
}

int TransactionId::hashCode() const {
	const uint8_t* ar = this->id->array();

	int ret = 0;

	int maxLoop = this->id->limit();
	for(int i = 0; i != maxLoop; ++i){
		uint8_t v = ar[i];

		ret += v;
	}

	return ret;
}

} /* namespace codablecash */
