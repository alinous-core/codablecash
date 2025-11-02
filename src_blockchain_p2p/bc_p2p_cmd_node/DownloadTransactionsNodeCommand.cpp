/*
 * DownloadTransactionsNodeCommand.cpp
 *
 *  Created on: 2023/11/27
 *      Author: iizuka
 */

#include "bc_p2p_cmd_node/DownloadTransactionsNodeCommand.h"
#include "bc_p2p_cmd_node/DownloadTransactionsNodeCommandResponse.h"
#include "bc_p2p_cmd_node/InvalidTransactionException.h"
#include "bc_p2p_cmd_node/DownloadTransactionEntry.h"

#include "bc_trx/TransactionId.h"
#include "bc_trx/AbstractControlTransaction.h"
#include "bc_trx/AbstractBalanceTransaction.h"
#include "bc_trx/AbstractInterChainCommunicationTansaction.h"

#include "base/StackRelease.h"

#include "bc/CodablecashNodeInstance.h"
#include "bc/ExceptionThrower.h"

#include "transaction/AbstractSmartcontractTransaction.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc_block/BlockHeaderId.h"
#include "bc_block/Block.h"

#include "bc_block_body/BlockBody.h"

#include "bc_base/BinaryUtils.h"


namespace codablecash {

DownloadTransactionsNodeCommand::DownloadTransactionsNodeCommand(const DownloadTransactionsNodeCommand &inst) : AbstractNodeCommand(inst) {
	this->height = inst.height;
	this->headerId = inst.headerId != nullptr ? dynamic_cast<BlockHeaderId*>(inst.headerId->copyData()) : nullptr;
	this->list = new ArrayList<DownloadTransactionEntry>();

	int maxLoop = inst.list->size();
	for(int i = 0; i != maxLoop; ++i){
		const DownloadTransactionEntry* trxEntry = inst.list->get(i);

		this->list->addElement(dynamic_cast<DownloadTransactionEntry*>(trxEntry->copyData()));
	}
}

DownloadTransactionsNodeCommand::DownloadTransactionsNodeCommand() : AbstractNodeCommand(TYPE_NODE_DOWNLOAD_TRANSACTIONS) {
	this->height = 0;
	this->headerId = nullptr;
	this->list = new ArrayList<DownloadTransactionEntry>();
}

DownloadTransactionsNodeCommand::~DownloadTransactionsNodeCommand() {
	this->list->deleteElements();
	delete this->list;
	delete this->headerId;
}

int DownloadTransactionsNodeCommand::binarySize() const {
	BinaryUtils::checkNotNull(this->headerId);

	int total = AbstractNodeCommand::binarySize();

	total += sizeof(this->height);
	total += this->headerId->binarySize();

	int maxLoop = this->list->size();
	total += sizeof(uint16_t);

	for(int i = 0; i != maxLoop; ++i){
		const DownloadTransactionEntry* trxEntry = this->list->get(i);

		total += trxEntry->binarySize();
	}

	return total;
}

void DownloadTransactionsNodeCommand::toBinary(ByteBuffer *buff) const {
	BinaryUtils::checkNotNull(this->headerId);

	AbstractNodeCommand::toBinary(buff);

	buff->putLong(this->height);
	this->headerId->toBinary(buff);

	int maxLoop = this->list->size();
	buff->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		const DownloadTransactionEntry* trxEntry = this->list->get(i);

		trxEntry->toBinary(buff);
	}
}

ByteBuffer* DownloadTransactionsNodeCommand::getSignBinary() const {
	BinaryUtils::checkNotNull(this->headerId);

	int total = sizeof(this->height);
	total += this->headerId->binarySize();

	{
		int maxLoop = this->list->size();
		total += sizeof(uint16_t);

		for(int i = 0; i != maxLoop; ++i){
			const DownloadTransactionEntry* trxEntry = this->list->get(i);

			total += trxEntry->binarySize();
		}
	}

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true);

	buff->putLong(this->height);
	this->headerId->toBinary(buff);

	int maxLoop = this->list->size();
	buff->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		const DownloadTransactionEntry* trxEntry = this->list->get(i);

		trxEntry->toBinary(buff);
	}

	return buff;
}

void DownloadTransactionsNodeCommand::fromBinary(ByteBuffer *buff) {
	AbstractNodeCommand::fromBinary(buff);

	this->height = buff->getLong();
	this->headerId = BlockHeaderId::fromBinary(buff);

	int maxLoop = buff->getShort();
	for(int i = 0; i != maxLoop; ++i){
		DownloadTransactionEntry* trxEntry = DownloadTransactionEntry::createFromBinary(buff);

		this->list->addElement(trxEntry);
	}
}

IBlockObject* DownloadTransactionsNodeCommand::copyData() const noexcept {
	return new DownloadTransactionsNodeCommand(*this);
}

bool DownloadTransactionsNodeCommand::usePendingQueue() const noexcept {
	return false;
}

AbstractCommandResponse* DownloadTransactionsNodeCommand::executeAsNode(BlockchainNodeHandshake *nodeHandShake, CodablecashNodeInstance *inst, bool suspend) const {
	BlockchainController* ctrl = inst->getController();
	uint16_t zone = inst->getZoneSelf();

	Block* block = ctrl->getBlockHeightAt(zone, this->height, this->headerId); __STP(block);
	ExceptionThrower<InvalidTransactionException>::throwExceptionIfCondition(block == nullptr
			, L"The block does not exists in the blockchain.", __FILE__, __LINE__);

	BlockBody* body = block->getBody();

	// download transaction
	DownloadTransactionsNodeCommandResponse* response = new DownloadTransactionsNodeCommandResponse(); __STP(response);

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		DownloadTransactionEntry* trxEntry = this->list->get(i);

		const AbstractBlockchainTransaction* trx = getTransaction(trxEntry, body);
		ExceptionThrower<InvalidTransactionException>::throwExceptionIfCondition(trx == nullptr
				, L"Transaction does not exists in Memory Pool.", __FILE__, __LINE__);

		response->addTransaction(trx);
	}

	return __STP_MV(response);

}

const AbstractBlockchainTransaction* DownloadTransactionsNodeCommand::getTransaction(const DownloadTransactionEntry *trxEntry, const BlockBody* body) const {
	const TransactionId* trxId = trxEntry->getTransactionId();

	uint8_t trxType = trxEntry->getTrxType();

	if(trxType == DownloadTransactionEntry::TYPE_CONTROL){
		return body->getControlTransaction(trxId);
	}
	else if(trxType == DownloadTransactionEntry::TYPE_ICC){
		return body->getInterChainCommunicationTansaction(trxId);
	}
	else if(trxType == DownloadTransactionEntry::TYPE_SMARTCONTRACT){
		return body->getSmartcontractTransaction(trxId);
	}

	return body->getBalanceTransaction(trxId);
}

void DownloadTransactionsNodeCommand::addTrxId(const DownloadTransactionEntry* trxEntry) {
	this->list->addElement(dynamic_cast<DownloadTransactionEntry*>(trxEntry->copyData()));
}

void DownloadTransactionsNodeCommand::setBlockHeaderId(const BlockHeaderId *headerId) {
	delete this->headerId;
	this->headerId = dynamic_cast<BlockHeaderId*>(headerId->copyData());
}

} /* namespace codablecash */
