/*
 * VoteTransactionIdCertificate.cpp
 *
 *  Created on: 2023/11/06
 *      Author: iizuka
 */

#include "data_history_data/VoteTransactionIdCertificate.h"

#include "bc_finalizer_trx/VoteBlockTransaction.h"

#include "merkletree/MerkleCertificate.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

namespace codablecash {

VoteTransactionIdCertificate::VoteTransactionIdCertificate(const VoteTransactionIdCertificate &inst) {
	this->voteTrx = inst.voteTrx != nullptr ? dynamic_cast<VoteBlockTransaction*>(inst.voteTrx->copyData()) : nullptr;
	this->cert = inst.cert != nullptr ? dynamic_cast<MerkleCertificate*>(inst.cert->copyData()) : nullptr;
}

VoteTransactionIdCertificate::VoteTransactionIdCertificate() {
	this->voteTrx = nullptr;
	this->cert = nullptr;
}

VoteTransactionIdCertificate::~VoteTransactionIdCertificate() {
	delete this->voteTrx;
	delete this->cert;
}

int VoteTransactionIdCertificate::binarySize() const {
	BinaryUtils::checkNotNull(this->voteTrx);
	BinaryUtils::checkNotNull(this->cert);

	int total = this->voteTrx->binarySize();
	total += this->cert->binarySize();

	return total;
}

void VoteTransactionIdCertificate::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->voteTrx);
	BinaryUtils::checkNotNull(this->cert);

	this->voteTrx->toBinary(out);
	this->cert->toBinary(out);
}

VoteTransactionIdCertificate* VoteTransactionIdCertificate::createFromBinary(ByteBuffer *in) {
	VoteTransactionIdCertificate* inst = new VoteTransactionIdCertificate(); __STP(inst);

	AbstractBlockchainTransaction* trx = AbstractBlockchainTransaction::createFromBinary(in); __STP(trx);
	inst->voteTrx = dynamic_cast<VoteBlockTransaction*>(trx);
	BinaryUtils::checkNotNull(inst->voteTrx);
	__STP_MV(trx);

	inst->cert = MerkleCertificate::createFromBinary(in);

	return __STP_MV(inst);
}

IBlockObject* VoteTransactionIdCertificate::copyData() const noexcept {
	return new VoteTransactionIdCertificate(*this);
}

void VoteTransactionIdCertificate::setVoteBlockTransaction(const VoteBlockTransaction *trx) {
	delete this->voteTrx;
	this->voteTrx = dynamic_cast<VoteBlockTransaction*>(trx->copyData());
}

void VoteTransactionIdCertificate::setMerkleCertificate(const MerkleCertificate *cert) {
	delete this->cert;
	this->cert = dynamic_cast<MerkleCertificate*>(cert->copyData());
}

} /* namespace codablecash */
