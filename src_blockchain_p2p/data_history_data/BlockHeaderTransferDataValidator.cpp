/*
 * BlockHeaderTransferDataValidator.cpp
 *
 *  Created on: 2023/11/12
 *      Author: iizuka
 */

#include "data_history_data/BlockHeaderTransferDataValidator.h"
#include "data_history_data/BlockHeaderTransferData.h"
#include "data_history_data/DataStructureException.h"
#include "data_history_data/VoteTransactionIdCertificate.h"

#include "bc/ExceptionThrower.h"

#include "bc_block/BlockHeader.h"
#include "bc_block/BlockHeaderId.h"

#include "merkletree/MerkleCertificate.h"

#include "base/StackRelease.h"

#include "bc_block/BlockMerkleRoot.h"

#include "bc_trx/TransactionId.h"


namespace codablecash {

BlockHeaderTransferDataValidator::BlockHeaderTransferDataValidator(const BlockHeaderTransferData* data) {
	this->data = data;
}

BlockHeaderTransferDataValidator::~BlockHeaderTransferDataValidator() {
	this->data = nullptr;
}

/**
 * for each vote transaction id, get certification, and check it
 */
void BlockHeaderTransferDataValidator::visit(const BlockHeaderId *votedHeaderId, const TransactionId *voteTrxId) {
	// get certification
	const VoteTransactionIdCertificate* certData = this->data->getVoteTransactionIdCertificate(voteTrxId);
	ExceptionThrower<DataStructureException>::throwExceptionIfCondition(certData == nullptr, L"No certification data.", __FILE__, __LINE__);

	const MerkleCertificate* merklecert = certData->getMerkleCertificate();

	// check markle root
	{
		const BlockHeader* header = this->data->getHeader();
		ByteBuffer* certRoot = merklecert->getRootHash(); __STP(certRoot);

		const BlockMerkleRoot* merkleRoot = header->getMerkleRoot();
		ByteBuffer* rootBuff = merkleRoot->getByteBuffer();

		bool bl = certRoot->binaryEquals(rootBuff);
		ExceptionThrower<DataStructureException>::throwExceptionIfCondition(!bl, L"Merkle root is wrong.", __FILE__, __LINE__);
	}

	// check certification
	{
		ByteBuffer* buff = voteTrxId->getByteBuffer();
		bool bl = merklecert->certificate(buff);
		ExceptionThrower<DataStructureException>::throwExceptionIfCondition(!bl, L"Merkle certification is wrong.", __FILE__, __LINE__);
	}
}

} /* namespace codablecash */
