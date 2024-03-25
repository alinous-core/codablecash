/*
 * TransferedDataId.cpp
 *
 *  Created on: 2023/09/18
 *      Author: iizuka
 */

#include "data_history/TransferedDataId.h"

#include "base/StackRelease.h"
namespace codablecash {

TransferedDataId::TransferedDataId(const TransferedDataId &inst) : Abstract32BytesId(inst) {
}

TransferedDataId::TransferedDataId() {

}

TransferedDataId::TransferedDataId(const char *binary, int length) : Abstract32BytesId(binary, length) {
}

TransferedDataId::~TransferedDataId() {

}

TransferedDataId* TransferedDataId::fromBinary(ByteBuffer *in) {
	TransferedDataId* transferedDataId = new TransferedDataId();

	int cap = in->getInt();

	uint8_t* buff = new uint8_t[cap];
	StackArrayRelease<uint8_t> __st_buff(buff);

	in->get(buff, cap);

	transferedDataId->id = ByteBuffer::wrapWithEndian(buff, cap, true);
	transferedDataId->id->position(0);

	return transferedDataId;
}

IBlockObject* TransferedDataId::copyData() const noexcept {
	return new TransferedDataId(*this);
}

} /* namespace codablecash */
