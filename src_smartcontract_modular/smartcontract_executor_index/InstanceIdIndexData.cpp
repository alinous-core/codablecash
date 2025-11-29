/*
 * InstanceIdIndexData.cpp
 *
 *  Created on: Nov 28, 2025
 *      Author: iizuka
 */

#include "smartcontract_executor_index/InstanceIdIndexData.h"

#include "trx/session/base/CdbDatabaseSessionId.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"


namespace codablecash {

InstanceIdIndexData::InstanceIdIndexData(const InstanceIdIndexData &inst) {
	this->trxId = inst.trxId != nullptr ? dynamic_cast<CdbDatabaseSessionId*>(inst.trxId->copyData()) : nullptr;
}

InstanceIdIndexData::InstanceIdIndexData() {
	this->trxId = nullptr;
}

InstanceIdIndexData::InstanceIdIndexData(CdbDatabaseSessionId *id) {
	this->trxId = id != nullptr ? dynamic_cast<CdbDatabaseSessionId*>(id->copyData()) : nullptr;
}

InstanceIdIndexData::~InstanceIdIndexData() {
	delete this->trxId;
}

int InstanceIdIndexData::binarySize() const {
	BinaryUtils::checkNotNull(this->trxId);

	return this->trxId->binarySize();
}

void InstanceIdIndexData::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->trxId);

	this->trxId->toBinary(out);
}

InstanceIdIndexData* InstanceIdIndexData::createFromBinary(ByteBuffer *in) {
	CdbDatabaseSessionId* id = CdbDatabaseSessionId::createFromBinary(in); __STP(id);

	return new InstanceIdIndexData(id);
}

IBlockObject* InstanceIdIndexData::copyData() const noexcept {
	return new InstanceIdIndexData(*this);
}

} /* namespace codablecash */
