/*
 * SyncHeaderNodeCommandResponse.cpp
 *
 *  Created on: 2023/11/06
 *      Author: iizuka
 */

#include "bc_p2p_cmd_node/SyncHeaderNodeCommandResponse.h"
#include "bc_p2p_cmd_node/SyncHeaderHeightData.h"

#include "data_history_data/BlockHeaderTransferData.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "bc_block/BlockHeader.h"

#include "bc_block/BlockHeaderId.h"


namespace codablecash {

SyncHeaderNodeCommandResponse::SyncHeaderNodeCommandResponse() : AbstractCommandResponse(TYPE_RES_SYNC_HEADER) {
	this->list = new ArrayList<SyncHeaderHeightData>();
}

SyncHeaderNodeCommandResponse::~SyncHeaderNodeCommandResponse() {
	this->list->deleteElements();
	delete this->list;
}

int SyncHeaderNodeCommandResponse::getDataCount() const noexcept {
	return this->list->size();
}

const SyncHeaderHeightData* SyncHeaderNodeCommandResponse::get(int i) const noexcept {
	return this->list->get(i);
}

int SyncHeaderNodeCommandResponse::binarySize() const {
	int total = sizeof(this->type);

	int maxLoop = this->list->size();
	total += sizeof(uint16_t);

	for(int i = 0; i != maxLoop; ++i){
		SyncHeaderHeightData* data = this->list->get(i);

		total += data->binarySize();
	}

	return total;
}

void SyncHeaderNodeCommandResponse::toBinary(ByteBuffer *buff) const {
	buff->putInt(this->type);

	int maxLoop = this->list->size();
	buff->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		SyncHeaderHeightData* data = this->list->get(i);

		data->toBinary(buff);
	}
}

void SyncHeaderNodeCommandResponse::fromBinary(ByteBuffer *buff) {
	int maxLoop = buff->getShort();
	for(int i = 0; i != maxLoop; ++i){
		SyncHeaderHeightData* data = SyncHeaderHeightData::createFromBinary(buff);

		this->list->addElement(data);
	}
}

UnicodeString* SyncHeaderNodeCommandResponse::toString() const noexcept {
	UnicodeString* ret = new UnicodeString(L""); __STP(ret);

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		SyncHeaderHeightData* data = this->list->get(i);

		if(i != 0){
			ret->append(L", ");
		}

		{
			UnicodeString* str = data->toString(); __STP(str);
			ret->append(str);
		}
	}

	return __STP_MV(ret);
}

void SyncHeaderNodeCommandResponse::detect(uint16_t height, const ArrayList<Block> *list) {
	SyncHeaderHeightData* data = new SyncHeaderHeightData(); __STP(data);
	data->setHeight(height);

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		Block* block = list->get(i);

		BlockHeaderTransferData* headerData = BlockHeaderTransferData::createFromBlock(block); __STP(headerData);
		data->addHeaderData(headerData);
	}

	this->list->addElement(__STP_MV(data));
}

} /* namespace codablecash */
