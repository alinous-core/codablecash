/*
 * OmittedBlockBody.cpp
 *
 *  Created on: 2023/11/23
 *      Author: iizuka
 */

#include "bc_block_body/OmittedBlockBody.h"
#include "bc_block_body/BlockRewordBase.h"
#include "bc_block_body/BlockBody.h"
#include "bc_block_body/OmittedBlockBodyFixer.h"

#include "bc_network/NodeIdentifierSource.h"

#include "bc_block/BlockMerkleRoot.h"

#include "bc_trx/TransactionId.h"
#include "bc_trx/AbstractBlockchainTransaction.h"
#include "bc_trx/AbstractBalanceTransaction.h"
#include "bc_trx/AbstractControlTransaction.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"
#include "base/HashMap.h"

#include "bc_p2p_cmd_node/DownloadTransactionsNodeCommand.h"
#include "bc_p2p_cmd_node/DownloadTransactionsNodeCommandResponse.h"
#include "bc_p2p_cmd_node/DownloadTransactionEntry.h"

#include "bc_memorypool/MemPoolTransaction.h"

#include "bc_trx/AbstractInterChainCommunicationTansaction.h"

#include "bc_p2p_cmd_node/InvalidTransactionException.h"

#include "bc/ExceptionThrower.h"

#include "bc_smartcontract/AbstractSmartcontractTransaction.h"


using namespace alinous;

namespace codablecash {

OmittedBlockBody::OmittedBlockBody(const OmittedBlockBody &inst) {
	this->nonce = inst.nonce;

	this->controlTransactions = new ArrayList<TransactionId>();
	{
		int maxLoop = inst.controlTransactions->size();
		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = inst.controlTransactions->get(i);

			this->controlTransactions->addElement(dynamic_cast<TransactionId*>(trxId->copyData()));
		}
	}

	this->iccTransactions = new ArrayList<TransactionId>();
	{
		int maxLoop = inst.iccTransactions->size();
		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = inst.iccTransactions->get(i);

			this->iccTransactions->addElement(dynamic_cast<TransactionId*>(trxId->copyData()));
		}
	}

	this->balanceTransactions = new ArrayList<TransactionId>();
	{
		int maxLoop = inst.balanceTransactions->size();
		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = inst.balanceTransactions->get(i);

			this->balanceTransactions->addElement(dynamic_cast<TransactionId*>(trxId->copyData()));
		}
	}

	this->smartcontractTransactions = new ArrayList<TransactionId>();
	{
		int maxLoop = inst.smartcontractTransactions->size();
		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = inst.smartcontractTransactions->get(i);

			this->smartcontractTransactions->addElement(dynamic_cast<TransactionId*>(trxId->copyData()));
		}
	}

	this->rewardBase = inst.rewardBase ? new BlockRewordBase(*inst.rewardBase) : nullptr;
}

OmittedBlockBody::OmittedBlockBody() {
	this->nonce = 0;

	this->controlTransactions = new ArrayList<TransactionId>();
	this->iccTransactions = new ArrayList<TransactionId>();
	this->balanceTransactions = new ArrayList<TransactionId>();
	this->smartcontractTransactions = new ArrayList<TransactionId>();

	this->rewardBase = nullptr;
}

OmittedBlockBody::~OmittedBlockBody() {
	this->controlTransactions->deleteElements();
	delete this->controlTransactions;

	this->iccTransactions->deleteElements();
	delete this->iccTransactions;

	this->balanceTransactions->deleteElements();
	delete this->balanceTransactions;

	this->smartcontractTransactions->deleteElements();
	delete this->smartcontractTransactions;

	delete this->rewardBase;
}

int OmittedBlockBody::binarySize() const {
	BinaryUtils::checkNotNull(this->rewardBase);

	int total = sizeof(this->nonce);

	{
		int maxLoop = this->controlTransactions->size();
		total += sizeof(uint16_t);

		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = this->controlTransactions->get(i);

			total += trxId->binarySize();
		}
	}

	{
		int maxLoop = this->iccTransactions->size();
		total += sizeof(uint16_t);

		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = this->iccTransactions->get(i);

			total += trxId->binarySize();
		}
	}

	{
		int maxLoop = this->balanceTransactions->size();
		total += sizeof(uint16_t);

		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = this->balanceTransactions->get(i);

			total += trxId->binarySize();
		}
	}

	{
		int maxLoop = this->smartcontractTransactions->size();
		total += sizeof(uint16_t);

		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = this->smartcontractTransactions->get(i);

			total += trxId->binarySize();
		}
	}

	total += this->rewardBase->binarySize();

	return total;
}

void OmittedBlockBody::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->rewardBase);

	out->putLong(this->nonce);

	{
		int maxLoop = this->controlTransactions->size();
		out->putShort(maxLoop);

		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = this->controlTransactions->get(i);

			trxId->toBinary(out);
		}
	}

	{
		int maxLoop = this->iccTransactions->size();
		out->putShort(maxLoop);

		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = this->iccTransactions->get(i);

			trxId->toBinary(out);
		}
	}

	{
		int maxLoop = this->balanceTransactions->size();
		out->putShort(maxLoop);

		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = this->balanceTransactions->get(i);

			trxId->toBinary(out);
		}
	}

	{
		int maxLoop = this->smartcontractTransactions->size();
		out->putShort(maxLoop);

		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = this->smartcontractTransactions->get(i);

			trxId->toBinary(out);
		}
	}

	this->rewardBase->toBinary(out);
}

OmittedBlockBody* OmittedBlockBody::createFromBinary(ByteBuffer *in) {
	OmittedBlockBody* body = new OmittedBlockBody(); __STP(body);

	body->nonce = in->getLong();

	{
		int maxLoop = in->getShort();
		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = TransactionId::fromBinary(in);

			body->controlTransactions->addElement(trxId);
		}
	}

	{
		int maxLoop = in->getShort();
		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = TransactionId::fromBinary(in);

			body->iccTransactions->addElement(trxId);
		}
	}

	{
		int maxLoop = in->getShort();
		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = TransactionId::fromBinary(in);

			body->balanceTransactions->addElement(trxId);
		}
	}

	{
		int maxLoop = in->getShort();
		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = TransactionId::fromBinary(in);

			body->smartcontractTransactions->addElement(trxId);
		}
	}

	body->rewardBase = BlockRewordBase::createFromBinary(in);

	return __STP_MV(body);
}

IBlockObject* OmittedBlockBody::copyData() const noexcept {
	return new OmittedBlockBody(*this);
}

void OmittedBlockBody::setNonce(uint64_t nonce) {
	this->nonce = nonce;
}

void OmittedBlockBody::addBalanceTransaction(const TransactionId *trxId) {
	TransactionId* t = dynamic_cast<TransactionId*>(trxId->copyData());
	this->balanceTransactions->addElement(t);
}

void OmittedBlockBody::addControlTransaction(const TransactionId *trxId) {
	TransactionId* t = dynamic_cast<TransactionId*>(trxId->copyData());
	this->controlTransactions->addElement(t);
}

void OmittedBlockBody::addInterChainCommunicationTransaction(const TransactionId *trxId) {
	TransactionId* t = dynamic_cast<TransactionId*>(trxId->copyData());
	this->iccTransactions->addElement(t);
}

void OmittedBlockBody::addSmartContractTransaction(const TransactionId *trxId) {
	TransactionId* t = dynamic_cast<TransactionId*>(trxId->copyData());
	this->smartcontractTransactions->addElement(t);
}

void OmittedBlockBody::setRewordBase(const BlockRewordBase *rewardBase) {
	delete this->rewardBase;
	this->rewardBase = new BlockRewordBase(*rewardBase);
}

BlockBody* OmittedBlockBody::toBlockBody(uint64_t height, const BlockHeaderId* headerId, IOmittedBlockBodyFixer* fixer, ISystemLogger *logger) const {
	BlockBody* body = new BlockBody(this->nonce); __STP(body);

	scanUnsentBalanceTransactions(height, headerId, body, fixer);
	scanUnsentControlTransactions(height, headerId, body, fixer);
	scanUnsentInterChainCommunicationTransactions(height, headerId, body, fixer);
	scanUnsentSmartcontractTransactions(height, headerId, body, fixer);

	fixer->downloadTransactions(logger);

	exportControlTransactions(height, headerId, body, fixer);
	exportInterChainCommunicationTransactions(height, headerId, body, fixer);
	exportBalanceTransactions(height, headerId, body, fixer);
	exportSmartcontractTransaction(height, headerId, body, fixer);

	body->setBlockRewordBase(this->rewardBase);

	body->build();
	return __STP_MV(body);
}

void OmittedBlockBody::scanUnsentBalanceTransactions(uint64_t height, const BlockHeaderId *headerId, BlockBody *body, IOmittedBlockBodyFixer* fixer) const {
	MemPoolTransaction* memTrx = fixer->begin(); __STP(memTrx);

	{
		int maxLoop = this->balanceTransactions->size();
		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = this->balanceTransactions->get(i);

			if(!memTrx->hasBalanceTransaction(trxId)){
				DownloadTransactionEntry entry(height, trxId, DownloadTransactionEntry::TYPE_BALANCE);
				fixer->addDownloadTransactionEntry(&entry);
			}
		}
	}
}

void OmittedBlockBody::scanUnsentControlTransactions(uint64_t height, const BlockHeaderId *headerId, BlockBody *body, IOmittedBlockBodyFixer* fixer) const {
	MemPoolTransaction* memTrx = fixer->begin(); __STP(memTrx);

	{
		int maxLoop = this->controlTransactions->size();
		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = this->controlTransactions->get(i);

			if(!memTrx->hasBalanceTransaction(trxId)){
				DownloadTransactionEntry entry(height, trxId, DownloadTransactionEntry::TYPE_CONTROL);
				fixer->addDownloadTransactionEntry(&entry);
			}
		}
	}
}

void OmittedBlockBody::scanUnsentInterChainCommunicationTransactions(uint64_t height, const BlockHeaderId *headerId, BlockBody *body, IOmittedBlockBodyFixer* fixer) const {
	MemPoolTransaction* memTrx = fixer->begin(); __STP(memTrx);

	{
		int maxLoop = this->iccTransactions->size();
		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = this->iccTransactions->get(i);

			if(!memTrx->hasBalanceTransaction(trxId)){
				DownloadTransactionEntry entry(height, trxId, DownloadTransactionEntry::TYPE_ICC);
				fixer->addDownloadTransactionEntry(&entry);
			}
		}
	}
}

void OmittedBlockBody::scanUnsentSmartcontractTransactions(uint64_t height, const BlockHeaderId *headerId, BlockBody *body, IOmittedBlockBodyFixer* fixer) const {
	MemPoolTransaction* memTrx = fixer->begin(); __STP(memTrx);

	{
		int maxLoop = this->smartcontractTransactions->size();
		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = this->smartcontractTransactions->get(i);

			if(!memTrx->hasBalanceTransaction(trxId)){
				DownloadTransactionEntry entry(height, trxId, DownloadTransactionEntry::TYPE_SMARTCONTRACT);
				fixer->addDownloadTransactionEntry(&entry);
			}
		}
	}
}

void OmittedBlockBody::exportBalanceTransactions(uint64_t height, const BlockHeaderId* headerId, BlockBody *body, IOmittedBlockBodyFixer* fixer) const {
	MemPoolTransaction* memTrx = fixer->begin(); __STP(memTrx);

	{
		int maxLoop = this->balanceTransactions->size();
		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = this->balanceTransactions->get(i);

			{
				const AbstractBalanceTransaction* trx = dynamic_cast<const AbstractBalanceTransaction*>(fixer->get(trxId));
				if(trx != nullptr){
					body->addBalanceTransaction(trx);
					continue;
				}
			}

			// from mempool
			AbstractBalanceTransaction* trx = memTrx->getBalanceTransaction(trxId); __STP(trx);
			body->addBalanceTransaction(trx);
		}
	}
}

void OmittedBlockBody::exportControlTransactions(uint64_t height, const BlockHeaderId* headerId, BlockBody *body, IOmittedBlockBodyFixer* fixer) const {
	MemPoolTransaction* memTrx = fixer->begin(); __STP(memTrx);

	{
		int maxLoop = this->controlTransactions->size();
		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = this->controlTransactions->get(i);

			{
				const AbstractControlTransaction* trx = dynamic_cast<const AbstractControlTransaction*>(fixer->get(trxId));
				if(trx != nullptr){
					body->addControlTransaction(trx);
					continue;
				}
			}

			// from mempool
			AbstractControlTransaction* trx = memTrx->getControlTransaction(trxId); __STP(trx);
			body->addControlTransaction(trx);
		}
	}
}

void OmittedBlockBody::exportInterChainCommunicationTransactions(uint64_t height, const BlockHeaderId* headerId, BlockBody *body, IOmittedBlockBodyFixer* fixer) const {
	MemPoolTransaction* memTrx = fixer->begin(); __STP(memTrx);

	{
		int maxLoop = this->iccTransactions->size();
		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = this->iccTransactions->get(i);

			{
				const AbstractInterChainCommunicationTansaction* trx = dynamic_cast<const AbstractInterChainCommunicationTansaction*>(fixer->get(trxId));
				if(trx != nullptr){
					body->addInterChainCommunicationTransaction(trx);
					continue;
				}
			}

			// from mempool
			AbstractInterChainCommunicationTansaction* trx = memTrx->getInterChainCommunicationTansaction(trxId); __STP(trx);
			body->addInterChainCommunicationTransaction(trx);
		}
	}
}

void OmittedBlockBody::exportSmartcontractTransaction(uint64_t height, const BlockHeaderId* headerId, BlockBody *body, IOmittedBlockBodyFixer* fixer) const {
	MemPoolTransaction* memTrx = fixer->begin(); __STP(memTrx);

	{
		int maxLoop = this->smartcontractTransactions->size();
		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = this->smartcontractTransactions->get(i);

			{
				const AbstractSmartcontractTransaction* trx = dynamic_cast<const AbstractSmartcontractTransaction*>(fixer->get(trxId));
				if(trx != nullptr){
					body->addSmartcontractTransaction(trx);
					continue;
				}
			}

			// from mempool
			AbstractSmartcontractTransaction* trx = memTrx->getSmartcontractTransaction(trxId); __STP(trx);
			body->addSmartcontractTransaction(trx);
		}
	}
}

} /* namespace codablecash */
