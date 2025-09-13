/*
 * ClientFetchHeaderTransactionsCommandResponse.cpp
 *
 *  Created on: Aug 20, 2025
 *      Author: iizuka
 */

#include "bc_p2p_cmd_client/ClientFetchHeaderTransactionsCommandResponse.h"
#include "bc_p2p_cmd_client/ClientBlockHeaderTransferData.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "bc_base/BinaryUtils.h"

#include "bc_block/BlockHeaderId.h"


namespace codablecash {

ClientFetchHeaderTransactionsCommandResponse::ClientFetchHeaderTransactionsCommandResponse()
		: AbstractCommandResponse(TYPE_RES_CLIENT_FETCH_HEADER_TRX) {
	this->transferData = nullptr;
}

ClientFetchHeaderTransactionsCommandResponse::~ClientFetchHeaderTransactionsCommandResponse() {
	delete this->transferData;
}

int ClientFetchHeaderTransactionsCommandResponse::binarySize() const {
	BinaryUtils::checkNotNull(this->transferData);

	int total = sizeof(this->type);

	total += this->transferData->binarySize();

	return total;
}

void ClientFetchHeaderTransactionsCommandResponse::toBinary(ByteBuffer *buff) const {
	BinaryUtils::checkNotNull(this->transferData);

	buff->putInt(this->type);

	this->transferData->toBinary(buff);
}

void ClientFetchHeaderTransactionsCommandResponse::fromBinary(ByteBuffer *buff) {
	AbstractTransferedData* data = AbstractTransferedData::createFromBinary(buff); __STP(data);

	this->transferData = dynamic_cast<ClientBlockHeaderTransferData*>(data);
	BinaryUtils::checkNotNull(this->transferData);

	__STP_MV(data);
}

UnicodeString* ClientFetchHeaderTransactionsCommandResponse::toString() const noexcept {
	UnicodeString* ret = new UnicodeString(L""); __STP(ret);

	const BlockHeader* header = this->transferData->getHeader();
	const BlockHeaderId* headerId = header->getId();

	UnicodeString* str = headerId->toString(); __STP(str);

	ret->append(L"ClientFetchHeaderTransactionsCommandResponse Header Id : ");
	ret->append(str);

	return __STP_MV(ret);
}

void ClientFetchHeaderTransactionsCommandResponse::setTransferData(ClientBlockHeaderTransferData *transferData) {
	delete this->transferData;
	this->transferData = transferData;
}

} /* namespace codablecash */
