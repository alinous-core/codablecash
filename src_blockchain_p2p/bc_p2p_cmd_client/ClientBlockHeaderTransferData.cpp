/*
 * ClientBlockHeaderTransferData.cpp
 *
 *  Created on: May 4, 2025
 *      Author: iizuka
 */

#include "bc_p2p_cmd_client/ClientBlockHeaderTransferData.h"
#include "bc_p2p_cmd_client/ClientBlockHeaderTransferDataValidator.h"

#include "data_history/TransferedDataId.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

#include "crypto/Sha256.h"

#include "data_history_data/BlockHeaderTransferDataValidator.h"
#include "data_history_data/DataStructureException.h"
#include "data_history_data/VoteTransactionIdCertificate.h"
#include "data_history_data/BlockHeaderTransferData.h"
#include "data_history_data/BlockTransactionTransferData.h"

#include "bc_finalizer_trx/VoteBlockTransaction.h"

#include "bc_trx/TransactionId.h"
#include "bc_trx/AbstractBlockchainTransaction.h"

#include "bc_block/Block.h"

#include "bc_block_body/BlockBody.h"

#include "bc/ExceptionThrower.h"

#include "merkletree/MerkleCertificate.h"


namespace codablecash {

ClientBlockHeaderTransferData::ClientBlockHeaderTransferData(const ClientBlockHeaderTransferData &inst) : AbstractTransferedData(inst) {
	this->header = inst.header != nullptr ? dynamic_cast<BlockHeader*>(inst.header->copyData()) : nullptr;

	this->certList = new ArrayList<VoteTransactionIdCertificate>();
	this->trxList = new ArrayList<BlockTransactionTransferData>();

	this->filtersList = nullptr;

	int maxLoop = inst.certList->size();
	for(int i = 0; i != maxLoop; ++i){
		const VoteTransactionIdCertificate* cert = inst.certList->get(i);

		addCert(cert);
	}

	maxLoop = inst.trxList->size();
	for(int i = 0; i != maxLoop; ++i){
		const BlockTransactionTransferData* data = inst.trxList->get(i);

		this->trxList->addElement(new BlockTransactionTransferData(*data));
	}
}

ClientBlockHeaderTransferData::ClientBlockHeaderTransferData() : AbstractTransferedData(DATA_CLIENT_BLOCKHEADER) {
	this->header = nullptr;
	this->certList = new ArrayList<VoteTransactionIdCertificate>();
	this->trxList = new ArrayList<BlockTransactionTransferData>();
	this->filtersList = nullptr;
}

ClientBlockHeaderTransferData::~ClientBlockHeaderTransferData() {
	this->certList->deleteElements();
	delete this->certList;

	this->trxList->deleteElements();
	delete this->trxList;

	delete this->header;

	this->filtersList = nullptr;
}

int ClientBlockHeaderTransferData::binarySize() const {
	BinaryUtils::checkNotNull(this->header);

	int total = sizeof(uint8_t); // type

	total += this->header->binarySize();

	int maxLoop = this->certList->size();
	total += sizeof(uint8_t);

	for(int i = 0; i != maxLoop; ++i){
		VoteTransactionIdCertificate* cert = this->certList->get(i);

		total += cert->binarySize();
	}

	maxLoop = this->trxList->size();
	total += sizeof(uint8_t);

	for(int i = 0; i != maxLoop; ++i){
		BlockTransactionTransferData* data = this->trxList->get(i);

		total += data->binarySize();
	}

	return total;
}

void ClientBlockHeaderTransferData::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->header);

	out->put(this->type);

	this->header->toBinary(out);

	int maxLoop = this->certList->size();
	out->put(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		VoteTransactionIdCertificate* cert = this->certList->get(i);

		cert->toBinary(out);
	}

	maxLoop = this->trxList->size();
	out->put(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		BlockTransactionTransferData* data = this->trxList->get(i);

		data->toBinary(out);
	}
}

void ClientBlockHeaderTransferData::fromBinary(ByteBuffer *in) {
	this->header = BlockHeader::createFromBinary(in);
	BinaryUtils::checkNotNull(this->header);

	int maxLoop = in->get();
	for(int i = 0; i != maxLoop; ++i){
		VoteTransactionIdCertificate* cert = VoteTransactionIdCertificate::createFromBinary(in);

		this->certList->addElement(cert);
	}

	maxLoop = in->get();
	for(int i = 0; i != maxLoop; ++i){
		AbstractTransferedData* data = AbstractTransferedData::createFromBinary(in); __STP(data);
		BlockTransactionTransferData* trxData = dynamic_cast<BlockTransactionTransferData*>(data);

		BinaryUtils::checkNotNull(trxData);
		__STP_MV(data);

		this->trxList->addElement(trxData);
	}
}

IBlockObject* ClientBlockHeaderTransferData::copyData() const noexcept {
	return new ClientBlockHeaderTransferData(*this);
}

void ClientBlockHeaderTransferData::addCert(const VoteTransactionIdCertificate *cert) noexcept {
	this->certList->addElement(dynamic_cast<VoteTransactionIdCertificate*>(cert->copyData()));
}

TransferedDataId* ClientBlockHeaderTransferData::getTransferedDataId() const {
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

void ClientBlockHeaderTransferData::detect(const Block *block, BlockHeaderId *votedHeaderId, TransactionId *voteTrxId) {
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

void ClientBlockHeaderTransferData::validateVoteTransactionsData() const {
	ClientBlockHeaderTransferDataValidator validator(this);

	this->header->visitVoteTransactionIdCertificate(&validator);
}

const VoteTransactionIdCertificate* ClientBlockHeaderTransferData::getVoteTransactionIdCertificate(const TransactionId *voteTrxId) const {
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

void ClientBlockHeaderTransferData::setHeader(const BlockHeader *header) noexcept {
	delete this->header;
	this->header = header != nullptr ? dynamic_cast<BlockHeader*>(header->copyData()) : nullptr;
}

BlockHeaderTransferData* ClientBlockHeaderTransferData::toBlockHeaderTransferData() const {
	BlockHeaderTransferData* data = new BlockHeaderTransferData(); __STP(data);
	data->setHeader(this->header);

	int maxLoop = this->certList->size();
	for(int i = 0; i != maxLoop; ++i){
		VoteTransactionIdCertificate* cert = this->certList->get(i);

		data->addCert(cert);
	}

	maxLoop = this->trxList->size();
	for(int i = 0; i != maxLoop; ++i){
		const BlockTransactionTransferData* trxData = this->trxList->get(i);

		data->addTransactionTransferData(trxData);
	}

	return __STP_MV(data);
}

ClientBlockHeaderTransferData* ClientBlockHeaderTransferData::createFromBlock(const Block *block) {
	ClientBlockHeaderTransferData* transferData = new ClientBlockHeaderTransferData(); __STP(transferData);

	const BlockHeader* header = block->getHeader();
	transferData->setHeader(header);

	header->buildVoteTransactionIdCertificate(block, transferData);

	return __STP_MV(transferData);
}

void ClientBlockHeaderTransferData::setFiltersList(const ArrayList<BloomFilter1024> *filtersList) {
	this->filtersList = filtersList;
}

void ClientBlockHeaderTransferData::visit(const AbstractBlockchainTransaction *trx, const Block* block) {
	if(trx->checkFilter(this->filtersList)){
		BlockBody* body = block->getBody();

		AbstractBlockchainTransaction* tx = dynamic_cast<AbstractBlockchainTransaction*>(trx->copyData()); __STP(tx);

		BlockTransactionTransferData* data = new BlockTransactionTransferData(); __STP(data);

		const TransactionId* trxId = tx->getTransactionId();
		MerkleCertificate* cert = body->makeCertificate(trxId); __STP(cert);

		data->setTransaction(trx, cert);

		this->trxList->addElement(__STP_MV(data));
	}
}

void ClientBlockHeaderTransferData::validateTransactions() const {
	const BlockMerkleRoot* merkleRoot = this->header->getMerkleRoot();

	int maxLoop = this->trxList->size();
	for(int i = 0; i != maxLoop; ++i){
		const BlockTransactionTransferData* data = this->trxList->get(i);

		data->validate(merkleRoot);
	}
}

} /* namespace codablecash */
