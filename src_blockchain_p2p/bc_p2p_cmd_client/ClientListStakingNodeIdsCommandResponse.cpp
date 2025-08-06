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


namespace codablecash {

ClientListStakingNodeIdsCommandResponse::ClientListStakingNodeIdsCommandResponse() : AbstractCommandResponse(AbstractCommandResponse::TYPE_RES_CLIENT_LIST_STAKING_IDS) {
	this->list = new ArrayList<NodeIdentifier>();

}

ClientListStakingNodeIdsCommandResponse::~ClientListStakingNodeIdsCommandResponse() {
	this->list->deleteElements();
	delete this->list;
}

int ClientListStakingNodeIdsCommandResponse::binarySize() const {
	int total = sizeof(this->type);

	int maxLoop = this->list->size();
	total += sizeof(uint16_t);

	for(int i = 0; i != maxLoop; ++i){
		const NodeIdentifier* nodeId = list->get(i);

		total += nodeId->binarySize();
	}

	return total;
}

void ClientListStakingNodeIdsCommandResponse::toBinary(ByteBuffer *buff) const {
	buff->putInt(this->type);

	int maxLoop = this->list->size();
	buff->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		const NodeIdentifier* nodeId = this->list->get(i);

		nodeId->toBinary(buff);
	}
}

void ClientListStakingNodeIdsCommandResponse::fromBinary(ByteBuffer *buff) {
	int maxLoop = buff->getShort();
	for(int i = 0; i != maxLoop; ++i){
		NodeIdentifier* nodeId = NodeIdentifier::fromBinary(buff);

		this->list->addElement(nodeId);
	}
}

UnicodeString* ClientListStakingNodeIdsCommandResponse::toString() const noexcept {
	UnicodeString* ret = new UnicodeString(L""); __STP(ret);

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		const NodeIdentifier* nodeId = this->list->get(i);
		UnicodeString* str = nodeId->toString(); __STP(str);

		ret->append(str);
	}

	return __STP_MV(ret);
}

} /* namespace codablecash */
