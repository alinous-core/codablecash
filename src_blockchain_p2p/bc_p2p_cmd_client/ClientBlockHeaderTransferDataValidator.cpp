/*
 * ClientBlockHeaderTransferDataValidator.cpp
 *
 *  Created on: May 4, 2025
 *      Author: iizuka
 */

#include "bc_p2p_cmd_client/ClientBlockHeaderTransferDataValidator.h"
#include "bc_p2p_cmd_client/ClientBlockHeaderTransferData.h"

#include "data_history_data/VoteTransactionIdCertificate.h"
#include "data_history_data/DataStructureException.h"

#include "bc/ExceptionThrower.h"

#include "bc_block/BlockHeader.h"
#include "bc_block/BlockMerkleRoot.h"

#include "base_io/ByteBuffer.h"

#include "merkletree/MerkleCertificate.h"

#include "base/StackRelease.h"

#include "bc_trx/TransactionId.h"

namespace codablecash {

ClientBlockHeaderTransferDataValidator::ClientBlockHeaderTransferDataValidator(const ClientBlockHeaderTransferData* data) {
	this->data = data;
}

ClientBlockHeaderTransferDataValidator::~ClientBlockHeaderTransferDataValidator() {
	this->data = nullptr;
}

void ClientBlockHeaderTransferDataValidator::visit(const BlockHeaderId *votedHeaderId, const TransactionId *voteTrxId) {
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
