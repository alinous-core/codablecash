/*
 * ClientListStakingNodeIdsCommandResponse.cpp
 *
 *  Created on: Aug 6, 2025
 *      Author: iizuka
 */

#include "bc_p2p_cmd_client/ClientListStakingNodeIdsCommandResponse.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "bc_finalizer/VoterEntry.h"

#include "bc_network/NodeIdentifier.h"

#include "merkletree/MerkleCertificate.h"

#include "bc/ExceptionThrower.h"
#include "filestore_block/exceptions.h"

#include "numeric/BigInteger.h"

using namespace alinous;

namespace codablecash {

ClientListStakingNodeIdsCommandResponse::ClientListStakingNodeIdsCommandResponse() : AbstractCommandResponse(AbstractCommandResponse::TYPE_RES_CLIENT_LIST_STAKING_IDS) {
	this->list = new ArrayList<NodeIdentifier>();
	this->certlist = new ArrayList<MerkleCertificate>();
}

ClientListStakingNodeIdsCommandResponse::~ClientListStakingNodeIdsCommandResponse() {
	this->list->deleteElements();
	delete this->list;

	this->certlist->deleteElements();
	delete this->certlist;
}

int ClientListStakingNodeIdsCommandResponse::binarySize() const {
	int total = sizeof(this->type);

	{
		int maxLoop = this->list->size();
		total += sizeof(uint16_t);

		for(int i = 0; i != maxLoop; ++i){
			const NodeIdentifier* nodeId = list->get(i);

			total += nodeId->binarySize();
		}
	}

	{
		int maxLoop = this->certlist->size();
		total += sizeof(uint16_t);

		for(int i = 0; i != maxLoop; ++i){
			const MerkleCertificate* cert = this->certlist->get(i);

			total += cert->binarySize();
		}
	}

	return total;
}

void ClientListStakingNodeIdsCommandResponse::toBinary(ByteBuffer *buff) const {
	buff->putInt(this->type);

	{
		int maxLoop = this->list->size();
		buff->putShort(maxLoop);

		for(int i = 0; i != maxLoop; ++i){
			const NodeIdentifier* nodeId = this->list->get(i);

			nodeId->toBinary(buff);
		}
	}

	{
		int maxLoop = this->certlist->size();
		buff->putShort(maxLoop);

		for(int i = 0; i != maxLoop; ++i){
			const MerkleCertificate* cert = this->certlist->get(i);

			cert->toBinary(buff);
		}
	}
}

void ClientListStakingNodeIdsCommandResponse::fromBinary(ByteBuffer *buff) {
	{
		int maxLoop = buff->getShort();
		for(int i = 0; i != maxLoop; ++i){
			NodeIdentifier* nodeId = NodeIdentifier::fromBinary(buff);

			this->list->addElement(nodeId);
		}
	}

	{
		int maxLoop = buff->getShort();
		for(int i = 0; i != maxLoop; ++i){
			MerkleCertificate* cert = MerkleCertificate::createFromBinary(buff);

			this->certlist->addElement(cert);
		}
	}

	ExceptionThrower<BinaryFormatException>::throwExceptionIfCondition(this->certlist->size() != this->list->size(), L"Differnt numbers of certificates list.", __FILE__, __LINE__);
}

UnicodeString* ClientListStakingNodeIdsCommandResponse::toString() const noexcept {
	UnicodeString* ret = new UnicodeString(L""); __STP(ret);

	{
		int maxLoop = this->list->size();
		for(int i = 0; i != maxLoop; ++i){
			const NodeIdentifier* nodeId = this->list->get(i);
			UnicodeString* str = nodeId->toString(); __STP(str);

			ret->append(str);
		}
	}

	return __STP_MV(ret);
}

void ClientListStakingNodeIdsCommandResponse::addNodeIdentifier(const NodeIdentifier *nodeId) noexcept {
	NodeIdentifier* newNodeId = dynamic_cast<NodeIdentifier*>(nodeId->copyData());
	this->list->addElement(newNodeId);
}

void ClientListStakingNodeIdsCommandResponse::addMerkleCertificate(const MerkleCertificate *cert) noexcept {
	MerkleCertificate* merkleCert = dynamic_cast<MerkleCertificate*>(cert->copyData());
	this->certlist->addElement(merkleCert);
}

void ClientListStakingNodeIdsCommandResponse::certifyNodeIds() {
	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		const NodeIdentifier* nodeId = this->list->get(i);
		const MerkleCertificate* cert = this->certlist->get(i);

		const BigInteger* pub = nodeId->getPublicKey();
		ByteBuffer* buff = pub->toBinary(); __STP(buff);

		bool bl = cert->certificate(buff);
		ExceptionThrower<BinaryFormatException>::throwExceptionIfCondition(!bl, L"Differnt numbers of certificates list.", __FILE__, __LINE__);
	}

}

} /* namespace codablecash */
