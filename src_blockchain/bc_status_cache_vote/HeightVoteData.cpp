/*
 * HeightVoteData.cpp
 *
 *  Created on: 2024/05/01
 *      Author: iizuka
 */

#include "bc_status_cache_vote/HeightVoteData.h"
#include "bc_status_cache_vote/VoterData.h"

#include "base_timestamp/SystemTimestamp.h"

namespace codablecash {

HeightVoteData::HeightVoteData(const HeightVoteData &inst) {
	this->timestamp = inst.timestamp != nullptr ? dynamic_cast<SystemTimestamp*>(inst.timestamp->copyData()) : nullptr;

	this->voters = new ArrayList<VoterData>();
	int maxLoop = inst.voters->size();
	for(int i = 0; i != maxLoop; ++i){
		VoterData* data = inst.voters->get(i);

		this->voters->addElement(new VoterData(*data));
	}
}

HeightVoteData::HeightVoteData() {
	this->timestamp = nullptr;
	this->voters = new ArrayList<VoterData>();
}

HeightVoteData::~HeightVoteData() {
	delete this->timestamp;

	this->voters->deleteElements();
	delete this->voters;
}

int HeightVoteData::binarySize() const {
	int total = sizeof(uint8_t);

	if(this->timestamp != nullptr){
		total += this->timestamp->binarySize();
	}

	total += sizeof(uint16_t);
	int maxLoop = this->voters->size();

	for(int i = 0; i != maxLoop; ++i){
		VoterData* data = this->voters->get(i);

		total += data->binarySize();
	}

	return total;
}

void HeightVoteData::toBinary(ByteBuffer *out) const {
	uint8_t bl = this->timestamp != nullptr ? 1 : 0;
	out->put(bl);

	if(this->timestamp != nullptr){
		this->timestamp->toBinary(out);
	}

	int maxLoop = this->voters->size();
	out->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		VoterData* data = this->voters->get(i);
		data->toBinary(out);
	}
}

HeightVoteData* HeightVoteData::fromBinary(ByteBuffer *in) {
	HeightVoteData* data = new HeightVoteData();

	uint8_t bl = in->get();
	if(bl > 0){
		data->timestamp = SystemTimestamp::fromBinary(in);
	}

	int maxLoop = in->getShort();
	for(int i = 0; i != maxLoop; ++i){
		VoterData* d = VoterData::createFromBinary(in);
		data->voters->addElement(d);
	}

	return data;
}

IBlockObject* HeightVoteData::copyData() const noexcept {
	return new HeightVoteData(*this);
}

void HeightVoteData::setTimestamp(const SystemTimestamp *tm) {
	delete this->timestamp;
	this->timestamp = dynamic_cast<SystemTimestamp*>(tm->copyData());
}

void HeightVoteData::validateTransactions() {
	int maxLoop = this->voters->size();
	for(int i = 0; i != maxLoop; ++i){
		VoterData* data = this->voters->get(i);

		data->validateByTimeLimit(timestamp);
	}
}

} /* namespace codablecash */
