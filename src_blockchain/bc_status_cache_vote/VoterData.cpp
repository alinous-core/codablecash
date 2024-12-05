/*
 * VoterData.cpp
 *
 *  Created on: 2024/05/01
 *      Author: iizuka
 */

#include "bc_status_cache_vote/VoterData.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_finalizer_trx/VoteBlockTransaction.h"

#include "base_io/ByteBuffer.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

#include "base_timestamp/SystemTimestamp.h"

namespace codablecash {

VoterData::VoterData(const VoterData &inst) {
	this->voterId = inst.voterId != nullptr ? dynamic_cast<NodeIdentifier*>(inst.voterId->copyData()) : nullptr;
	this->list = new ArrayList<VoteBlockTransaction>();

	int maxLoop = inst.list->size();
	for(int i = 0; i != maxLoop; ++i){
		VoteBlockTransaction* trx = inst.list->get(i);

		this->list->addElement(dynamic_cast<VoteBlockTransaction*>(trx->copyData()));
	}
}

VoterData::VoterData() {
	this->voterId = nullptr;
	this->list = new ArrayList<VoteBlockTransaction>();
}

VoterData::~VoterData() {
	delete this->voterId;

	this->list->deleteElements();
	delete this->list;
}

int VoterData::binarySize() const {
	BinaryUtils::checkNotNull(this->voterId);

	int total = this->voterId->binarySize();

	total += sizeof(uint16_t);
	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		VoteBlockTransaction* trx = this->list->get(i);

		total += trx->binarySize();
	}

	return total;
}

void VoterData::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->voterId);

	this->voterId->toBinary(out);

	int maxLoop = this->list->size();
	out->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		VoteBlockTransaction* trx = this->list->get(i);

		trx->toBinary(out);
	}
}

VoterData* VoterData::createFromBinary(ByteBuffer *in) {
	VoterData* data = new VoterData();

	data->voterId = NodeIdentifier::fromBinary(in);

	int maxLoop = in->getShort();
	BinaryUtils::checkUShortRange(maxLoop, 0, 10);
	for(int i = 0; i != maxLoop; ++i){
		AbstractBlockchainTransaction* atrx = AbstractBlockchainTransaction::createFromBinary(in); __STP(atrx);
		VoteBlockTransaction* trx = dynamic_cast<VoteBlockTransaction*>(atrx);

		BinaryUtils::checkNotNull(trx);
		__STP_MV(atrx);
		data->list->addElement(trx);
	}

	return data;
}

void VoterData::validateByTimeLimit(const SystemTimestamp *timestamp) {
	if(timestamp != nullptr){
		ArrayList<VoteBlockTransaction> removelist;

		// check
		int maxLoop = this->list->size();
		for(int i = 0; i != maxLoop; ++i){
			VoteBlockTransaction* trx = this->list->get(i);

			const SystemTimestamp* tm = trx->getTimestamp();
			if(tm->compareTo(timestamp) > 0){
				removelist.addElement(trx);
			}
		}

		// remove
		maxLoop = removelist.size();
		for(int i = 0; i != maxLoop; ++i){
			VoteBlockTransaction* trx = removelist.get(i); __STP(trx);

			this->list->removeByObj(trx);
		}
	}
}

} /* namespace codablecash */
