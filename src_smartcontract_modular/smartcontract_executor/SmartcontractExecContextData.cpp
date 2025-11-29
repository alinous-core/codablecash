/*
 * SmartcontractExecContextData.cpp
 *
 *  Created on: Nov 28, 2025
 *      Author: iizuka
 */

#include "smartcontract_executor/SmartcontractExecContextData.h"

#include "bc_trx/TransactionId.h"

#include "trx/session/base/CdbDatabaseSessionId.h"

#include "bc_block/BlockHeaderId.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

namespace codablecash {

SmartcontractExecContextData::SmartcontractExecContextData(const SmartcontractExecContextData &inst) {
	this->height = inst.height;

	this->lastHeight = inst.lastHeight;
	this->lastBlockHeaderId = inst.lastBlockHeaderId != nullptr ? dynamic_cast<BlockHeaderId*>(inst.lastBlockHeaderId->copyData()) : nullptr;
	this->lastTrxId = inst.lastTrxId != nullptr ? dynamic_cast<CdbDatabaseSessionId*>(inst.lastTrxId->copyData()) : nullptr;
	this->finalized = false;
}

SmartcontractExecContextData::SmartcontractExecContextData() {
	this->height = 0;

	this->lastHeight = 0;
	this->lastBlockHeaderId = nullptr;
	this->lastTrxId = nullptr;
	this->finalized = false;
}

SmartcontractExecContextData::~SmartcontractExecContextData() {
	delete this->lastBlockHeaderId;
	delete this->lastTrxId;
}

int SmartcontractExecContextData::binarySize() const {
	BinaryUtils::checkNotNull(this->lastBlockHeaderId);
	BinaryUtils::checkNotNull(this->lastTrxId);

	int total = sizeof(this->height);

	total += sizeof(this->lastHeight);
	total += this->lastBlockHeaderId->binarySize();
	total += this->lastTrxId->binarySize();
	total += sizeof(uint8_t);

	return total;
}

void SmartcontractExecContextData::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->lastBlockHeaderId);
	BinaryUtils::checkNotNull(this->lastTrxId);

	out->putLong(this->height);

	out->putLong(this->lastHeight);
	this->lastBlockHeaderId->toBinary(out);
	this->lastTrxId->toBinary(out);
	out->put(this->finalized ? 1 : 0);
}

SmartcontractExecContextData* SmartcontractExecContextData::createFromBinary(ByteBuffer *in) {
	SmartcontractExecContextData* inst = new SmartcontractExecContextData(); __STP(inst);

	inst->height = in->getLong();

	inst->lastHeight = in->getLong();
	inst->lastBlockHeaderId = BlockHeaderId::fromBinary(in);
	inst->lastTrxId = CdbDatabaseSessionId::createFromBinary(in);

	uint8_t bl = in->get();
	inst->finalized = (bl > 0);

	return __STP_MV(inst);
}

IBlockObject* SmartcontractExecContextData::copyData() const noexcept {
	return new SmartcontractExecContextData(*this);
}

} /* namespace codablecash */
