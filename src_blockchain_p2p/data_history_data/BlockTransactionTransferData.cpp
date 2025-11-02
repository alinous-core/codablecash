/*
 * BlockTransactionTransferData.cpp
 *
 *  Created on: Jun 15, 2025
 *      Author: iizuka
 */

#include "data_history_data/BlockTransactionTransferData.h"

#include "data_history/TransferedDataId.h"

#include "bc_trx/AbstractBlockchainTransaction.h"
#include "bc_trx/TransactionId.h"

#include "bc_base/BinaryUtils.h"

#include "merkletree/MerkleCertificate.h"

#include "base/StackRelease.h"

#include "bc/ExceptionThrower.h"

#include "data_history_data/DataStructureException.h"

#include "bc_block/BlockMerkleRoot.h"
namespace codablecash {

BlockTransactionTransferData::BlockTransactionTransferData(const BlockTransactionTransferData &inst) : AbstractTransferedData(inst) {
	this->trx = inst.trx != nullptr ? dynamic_cast<AbstractBlockchainTransaction*>(inst.trx->copyData()) : nullptr;
	this->cert = inst.cert != nullptr ? dynamic_cast<MerkleCertificate*>(inst.cert->copyData()) : nullptr;
}

BlockTransactionTransferData::BlockTransactionTransferData() : AbstractTransferedData(DATA_BLOCK_TRANSACTION){
	this->trx = nullptr;
	this->cert = nullptr;
}

BlockTransactionTransferData::~BlockTransactionTransferData() {
	delete this->trx;
	delete this->cert;
}

int BlockTransactionTransferData::binarySize() const {
	BinaryUtils::checkNotNull(this->trx);
	BinaryUtils::checkNotNull(this->cert);

	int total = sizeof(uint8_t);
	total += this->trx->binarySize();

	total += this->cert->binarySize();

	return total;
}

void BlockTransactionTransferData::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->trx);
	BinaryUtils::checkNotNull(this->cert);

	out->put(this->type);
	this->trx->toBinary(out);
	this->cert->toBinary(out);
}

void BlockTransactionTransferData::fromBinary(ByteBuffer *in) {
	this->trx = AbstractBlockchainTransaction::createFromBinary(in);
	BinaryUtils::checkNotNull(this->trx);

	this->cert = MerkleCertificate::createFromBinary(in);
	BinaryUtils::checkNotNull(this->cert);
}

TransferedDataId* BlockTransactionTransferData::getTransferedDataId() const {
	const TransactionId* id = this->trx->getTransactionId();

	TransferedDataId* dataId = new TransferedDataId(id->toArray(), id->size());
	return dataId;
}

IBlockObject* BlockTransactionTransferData::copyData() const noexcept {
	return new BlockTransactionTransferData(*this);
}

void BlockTransactionTransferData::setTransaction(const AbstractBlockchainTransaction *trx, const MerkleCertificate *cert) {
	delete this->trx, this->trx = nullptr;
	delete this->cert, this->cert = nullptr;

	this->trx = dynamic_cast<AbstractBlockchainTransaction*>(trx->copyData());
	this->cert = dynamic_cast<MerkleCertificate*>(cert->copyData());
}

void BlockTransactionTransferData::validate(const BlockMerkleRoot *merkleRoot) const {
	// check markle root
	{
		ByteBuffer* rootBuff = merkleRoot->getByteBuffer();
		ByteBuffer* certRoot = this->cert->getRootHash(); __STP(certRoot);

		bool bl = certRoot->binaryEquals(rootBuff);
		ExceptionThrower<DataStructureException>::throwExceptionIfCondition(!bl, L"Transaction's Merkle root is wrong.", __FILE__, __LINE__);
	}

	// check certification
	{
		const TransactionId* trxId = this->trx->getTransactionId();
		ByteBuffer* buff = trxId->getByteBuffer();

		bool bl = this->cert->certificate(buff);
		ExceptionThrower<DataStructureException>::throwExceptionIfCondition(!bl, L"Transaction's Merkle certification is wrong.", __FILE__, __LINE__);
	}
}

} /* namespace codablecash */
