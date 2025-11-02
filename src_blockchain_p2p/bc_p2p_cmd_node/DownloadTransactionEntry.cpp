/*
 * DownloadTransactionEntry.cpp
 *
 *  Created on: 2023/12/06
 *      Author: iizuka
 */

#include "bc_p2p_cmd_node/DownloadTransactionEntry.h"

#include "bc_trx/TransactionId.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

namespace codablecash {

DownloadTransactionEntry::DownloadTransactionEntry(uint64_t height, const TransactionId* trxId, uint8_t trxType) {
	this->height = height;
	this->trxId = trxId != nullptr ? dynamic_cast<TransactionId*>(trxId->copyData()) : nullptr;
	this->trxType = trxType;
}

DownloadTransactionEntry::~DownloadTransactionEntry() {
	delete this->trxId;
}

int DownloadTransactionEntry::binarySize() const {
	BinaryUtils::checkNotNull(this->trxId);

	int total = sizeof(this->height);
	total += this->trxId->binarySize();
	total += sizeof(this->trxType);

	return total;
}

void DownloadTransactionEntry::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->trxId);

	out->putLong(this->height);
	this->trxId->toBinary(out);
	out->put(this->trxType);
}

DownloadTransactionEntry* DownloadTransactionEntry::createFromBinary(ByteBuffer *in) {
	uint64_t height = in->getLong();
	TransactionId* trxId = TransactionId::fromBinary(in); __STP(trxId);
	uint8_t trxType = in->get();

	return new DownloadTransactionEntry(height, trxId, trxType);
}

IBlockObject* DownloadTransactionEntry::copyData() const noexcept {
	return new DownloadTransactionEntry(this->height, this->trxId, this->trxType);
}

} /* namespace codablecash */
