/*
 * ClientListStakingNodeIds.cpp
 *
 *  Created on: Aug 5, 2025
 *      Author: iizuka
 */

#include "bc_network/NodeIdentifier.h"

#include "base/StackRelease.h"

#include "bc_p2p_cmd_client/ClientListStakingNodeIdsCommand.h"

#include "bc/CodablecashNodeInstance.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc_status_cache_context/IStatusCacheContext.h"

#include "bc_p2p_cmd_client/ClientListStakingNodeIdsCommandResponse.h"
namespace codablecash {

ClientListStakingNodeIdsCommand::ClientListStakingNodeIdsCommand(const ClientListStakingNodeIdsCommand &inst) : AbstractClientRequestCommand(inst) {
	this->zone = inst.zone;
}

ClientListStakingNodeIdsCommand::ClientListStakingNodeIdsCommand() : AbstractClientRequestCommand(TYPE_CLIENT_LIST_STAKING_IDS) {
	this->zone = 0;
}

ClientListStakingNodeIdsCommand::~ClientListStakingNodeIdsCommand() {

}

int ClientListStakingNodeIdsCommand::binarySize() const {
	int total = AbstractClientRequestCommand::binarySize();

	total += sizeof(this->zone);

	return total;
}

void ClientListStakingNodeIdsCommand::toBinary(ByteBuffer *buff) const {
	AbstractClientRequestCommand::toBinary(buff);

	buff->putShort(this->zone);
}

void ClientListStakingNodeIdsCommand::fromBinary(ByteBuffer *buff) {
	AbstractClientRequestCommand::fromBinary(buff);

	this->zone = buff->getShort();
}

AbstractCommandResponse* ClientListStakingNodeIdsCommand::executeAsClient(ClientNodeHandshake *clientHandshake, CodablecashNodeInstance *inst) const {
	BlockchainController* ctrl = inst->getController();

	IStatusCacheContext* context = ctrl->getStatusCacheContext(this->zone); __STP(context);
	ArrayList<VoterEntry, VoterEntry::VoteCompare>* list = context->getVoterEntries(); __STP(list);
	list->setDeleteOnExit();

	ClientListStakingNodeIdsCommandResponse* response = new ClientListStakingNodeIdsCommandResponse(); __STP(response);

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		const VoterEntry* entry = list->get(i);
		const NodeIdentifier* nodeId = entry->getNodeIdentifier();
		const MerkleCertificate* cert = entry->getNodeIdMerkleCertificate();

		response->addNodeIdentifier(nodeId);
		response->addMerkleCertificate(cert);
	}

	return __STP_MV(response);
}

ByteBuffer* ClientListStakingNodeIdsCommand::getSignBinary() const {
	int total = sizeof(this->zone);

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true); __STP(buff);
	buff->putShort(this->zone);

	return __STP_MV(buff);
}

IBlockObject* ClientListStakingNodeIdsCommand::copyData() const noexcept {
	return new ClientListStakingNodeIdsCommand(*this);
}

void ClientListStakingNodeIdsCommand::setZone(uint16_t zone) noexcept {
	this->zone = zone;
}

} /* namespace codablecash */
