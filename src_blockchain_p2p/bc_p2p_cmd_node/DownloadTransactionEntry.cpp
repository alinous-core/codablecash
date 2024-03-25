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

DownloadTransactionEntry::DownloadTransactionEntry(uint64_t height, const TransactionId* trxId) {
	this->height = height;
	this->trxId = trxId != nullptr ? dynamic_cast<TransactionId*>(trxId->copyData()) : nullptr;
}

DownloadTransactionEntry::~DownloadTransactionEntry() {
	delete this->trxId;
}

int DownloadTransactionEntry::binarySize() const {
	BinaryUtils::checkNotNull(this->trxId);

	int total = sizeof(this->height);
	total += this->trxId->binarySize();

	return total;
}

void DownloadTransactionEntry::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->trxId);

	out->putLong(this->height);
	this->trxId->toBinary(out);
}

DownloadTransactionEntry* DownloadTransactionEntry::createFromBinary(ByteBuffer *in) {
	uint64_t height = in->getLong();
	TransactionId* trxId = TransactionId::fromBinary(in); __STP(trxId);

	return new DownloadTransactionEntry(height, trxId);
}

IBlockObject* DownloadTransactionEntry::copyData() const noexcept {
	return new DownloadTransactionEntry(this->height, this->trxId);
}

} /* namespace codablecash */
