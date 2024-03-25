/*
 * BlockHeaderTransferData.cpp
 *
 *  Created on: 2023/11/06
 *      Author: iizuka
 */

#include "data_history_data/BlockHeaderTransferData.h"
#include "data_history_data/VoteTransactionIdCertificate.h"

#include "data_history/TransferedDataId.h"

#include "data_history_data/DataStructureException.h"

#include "bc_block/BlockHeader.h"
#include "bc_block/Block.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

#include "crypto/Sha256.h"

#include "bc_block_body/BlockBody.h"

#include "bc/ExceptionThrower.h"

#include "bc_finalizer_trx/VoteBlockTransaction.h"

#include "merkletree/MerkleCertificate.h"

#include "bc_trx/TransactionId.h"

#include "data_history_data/BlockHeaderTransferDataValidator.h"
namespace codablecash {

BlockHeaderTransferData::BlockHeaderTransferData(const BlockHeaderTransferData &inst) : AbstractTransferedData(inst) {
	this->header = inst.header != nullptr ? dynamic_cast<BlockHeader*>(inst.header->copyData()) : nullptr;

	this->certList = new ArrayList<VoteTransactionIdCertificate>();

	int maxLoop = inst.certList->size();
	for(int i = 0; i != maxLoop; ++i){
		const VoteTransactionIdCertificate* cert = inst.certList->get(i);

		addCert(cert);
	}
}

BlockHeaderTransferData::BlockHeaderTransferData() : AbstractTransferedData(DATA_BLOCKHEADER) {
	this->header = nullptr;
	this->certList = new ArrayList<VoteTransactionIdCertificate>();
}

BlockHeaderTransferData::~BlockHeaderTransferData() {
	this->certList->deleteElements();
	delete this->certList;

	delete this->header;
}

int BlockHeaderTransferData::binarySize() const {
	BinaryUtils::checkNotNull(this->header);

	int total = sizeof(uint8_t); // type

	total += this->header->binarySize();

	int maxLoop = this->certList->size();
	total += sizeof(uint8_t);

	for(int i = 0; i != maxLoop; ++i){
		VoteTransactionIdCertificate* cert = this->certList->get(i);

		total += cert->binarySize();
	}

	return total;
}

void BlockHeaderTransferData::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->header);

	out->put(this->type);

	this->header->toBinary(out);

	int maxLoop = this->certList->size();
	out->put(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		VoteTransactionIdCertificate* cert = this->certList->get(i);

		cert->toBinary(out);
	}
}

void BlockHeaderTransferData::fromBinary(ByteBuffer *in) {
	this->header = BlockHeader::createFromBinary(in);
	BinaryUtils::checkNotNull(this->header);

	int maxLoop = in->get();
	for(int i = 0; i != maxLoop; ++i){
		VoteTransactionIdCertificate* cert = VoteTransactionIdCertificate::createFromBinary(in);

		this->certList->addElement(cert);
	}
}

IBlockObject* BlockHeaderTransferData::copyData() const noexcept {
	return new BlockHeaderTransferData(*this);
}

TransferedDataId* BlockHeaderTransferData::getTransferedDataId() const {
	BinaryUtils::checkNotNull(this->header);

	int total = binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true); __STP(buff);
	toBinary(buff);
	buff->position(0);

	ByteBuffer* sha = Sha256::sha256(buff, true); __STP(sha);
	sha->position(0);

	TransferedDataId* dataId = new TransferedDataId((const char*)sha->array(), sha->limit());
	return dataId;
}

void BlockHeaderTransferData::setHeader(const BlockHeader *header) noexcept {
	delete this->header;
	this->header = header != nullptr ? dynamic_cast<BlockHeader*>(header->copyData()) : nullptr;
}

void BlockHeaderTransferData::addCert(	const VoteTransactionIdCertificate *cert) noexcept {
	this->certList->addElement(dynamic_cast<VoteTransactionIdCertificate*>(cert->copyData()));
}

BlockHeaderTransferData* BlockHeaderTransferData::createFromBlock(const Block *block) {
	BlockHeaderTransferData* transferData = new BlockHeaderTransferData(); __STP(transferData);

	const BlockHeader* header = block->getHeader();
	transferData->setHeader(header);

	header->buildVoteTransactionIdCertificate(block, transferData);

	return __STP_MV(transferData);
}

void BlockHeaderTransferData::detect(const Block *block, BlockHeaderId *votedHeaderId, TransactionId* voteTrxId) {
	BlockBody* body = block->getBody();

	const AbstractBlockchainTransaction* transaction = body->getControlTransaction(voteTrxId);
	ExceptionThrower<DataStructureException>::throwExceptionIfCondition(transaction == nullptr, L"Voted transaction does not exists", __FILE__, __LINE__);

	const VoteBlockTransaction* voteTrx = dynamic_cast<const VoteBlockTransaction*>(transaction);
	ExceptionThrower<DataStructureException>::throwExceptionIfCondition(voteTrx == nullptr, L"Transaction type of the id is not Voted transaction", __FILE__, __LINE__);

	MerkleCertificate* certificate = body->makeCertificate(voteTrxId); __STP(certificate);

	VoteTransactionIdCertificate* trxIdCert = new VoteTransactionIdCertificate(); __STP(trxIdCert);
	trxIdCert->setVoteBlockTransaction(voteTrx);
	trxIdCert->setMerkleCertificate(certificate);

	this->addCert(trxIdCert);
}

void BlockHeaderTransferData::validateVoteTransactionsData() const {
	BlockHeaderTransferDataValidator validator(this);

	this->header->visitVoteTransactionIdCertificate(&validator);
}

const VoteTransactionIdCertificate* BlockHeaderTransferData::getVoteTransactionIdCertificate(const TransactionId *voteTrxId) const {
	VoteTransactionIdCertificate* ret = nullptr;

	int maxLoop = this->certList->size();
	for(int i = 0; i != maxLoop; ++i){
		VoteTransactionIdCertificate* cert = this->certList->get(i);
		const VoteBlockTransaction* trx = cert->getVoteBlockTransaction();
		const TransactionId* trxId = trx->getTransactionId();

		if(voteTrxId->equals(trxId)){
			ret = cert;
			break;
		}
	}

	return ret;
}

} /* namespace codablecash */
