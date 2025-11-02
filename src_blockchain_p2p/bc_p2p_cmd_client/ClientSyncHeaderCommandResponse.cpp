/*
 * ClientSyncHeaderCommandResponse.cpp
 *
 *  Created on: May 4, 2025
 *      Author: iizuka
 */

#include "bc_p2p_cmd_client/ClientSyncHeaderCommandResponse.h"
#include "bc_p2p_cmd_client/ClientSyncHeaderHeightData.h"
#include "bc_p2p_cmd_client/ClientBlockHeaderTransferData.h"

#include "base_io/ByteBuffer.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"


namespace codablecash {

ClientSyncHeaderCommandResponse::ClientSyncHeaderCommandResponse() : AbstractCommandResponse(TYPE_RES_CLIENT_SYNC_HEADER) {
	this->list = new ArrayList<ClientSyncHeaderHeightData>();
}

ClientSyncHeaderCommandResponse::~ClientSyncHeaderCommandResponse() {
	this->list->deleteElements();
	delete this->list;
}

int ClientSyncHeaderCommandResponse::binarySize() const {
	int total = sizeof(this->type);

	int maxLoop = this->list->size();
	total += sizeof(uint16_t);

	for(int i = 0; i != maxLoop; ++i){
		ClientSyncHeaderHeightData* data = this->list->get(i);

		total += data->binarySize();
	}

	return total;
}

void ClientSyncHeaderCommandResponse::toBinary(ByteBuffer *buff) const {
	buff->putInt(this->type);

	int maxLoop = this->list->size();
	buff->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		ClientSyncHeaderHeightData* data = this->list->get(i);

		data->toBinary(buff);
	}
}

void ClientSyncHeaderCommandResponse::fromBinary(ByteBuffer *buff) {
	int maxLoop = buff->getShort();
	for(int i = 0; i != maxLoop; ++i){
		ClientSyncHeaderHeightData* data = ClientSyncHeaderHeightData::createFromBinary(buff);

		this->list->addElement(data);
	}
}

UnicodeString* ClientSyncHeaderCommandResponse::toString() const noexcept {
	UnicodeString* ret = new UnicodeString(L""); __STP(ret);

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		ClientSyncHeaderHeightData* data = this->list->get(i);

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

void ClientSyncHeaderCommandResponse::detect(uint16_t height, const ArrayList<Block> *list) {
	ClientSyncHeaderHeightData* data = new ClientSyncHeaderHeightData(); __STP(data);
	data->setHeight(height);

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		Block* block = list->get(i);

		ClientBlockHeaderTransferData* headerData = ClientBlockHeaderTransferData::createFromBlock(block); __STP(headerData);
		data->addHeaderData(headerData);
	}

	this->list->addElement(__STP_MV(data));

}

int ClientSyncHeaderCommandResponse::getDataCount() const noexcept {
	return this->list->size();
}

const ClientSyncHeaderHeightData* ClientSyncHeaderCommandResponse::get(int i) const noexcept {
	return this->list->get(i);
}

} /* namespace codablecash */
