/*
 * ClientNotifyBlockMinedCommand.cpp
 *
 *  Created on: Aug 18, 2025
 *      Author: iizuka
 */

#include "bc_p2p_cmd_client_notify/ClientNotifyBlockMinedCommand.h"
#include "bc_p2p_cmd_client_notify/ClientExecutor.h"

#include "bc_block/BlockHeader.h"

#include "bc_base/BinaryUtils.h"

#include "data_history_data/BlockHeaderTransferData.h"

#include "base/StackRelease.h"

#include "data_history_data/TransactionTransferData.h"
namespace codablecash {

ClientNotifyBlockMinedCommand::ClientNotifyBlockMinedCommand(const ClientNotifyBlockMinedCommand &inst) : AbstractClientNotifyCommand(inst) {
	this->headerData = inst.headerData != nullptr ? dynamic_cast<BlockHeaderTransferData*>(inst.headerData->copyData()) : nullptr;
}

ClientNotifyBlockMinedCommand::ClientNotifyBlockMinedCommand() : AbstractClientNotifyCommand(TYPE_CLIENT_NOTIFY_BLOCK_MINED) {
	this->headerData = nullptr;
}

ClientNotifyBlockMinedCommand::~ClientNotifyBlockMinedCommand() {
	delete this->headerData;
}

int ClientNotifyBlockMinedCommand::binarySize() const {
	BinaryUtils::checkNotNull(this->headerData);

	int total = AbstractClientNotifyCommand::binarySize();
	total += this->headerData->binarySize();

	return total;
}

void ClientNotifyBlockMinedCommand::toBinary(ByteBuffer *buff) const {
	BinaryUtils::checkNotNull(this->headerData);

	AbstractClientNotifyCommand::toBinary(buff);
	this->headerData->toBinary(buff);
}

void ClientNotifyBlockMinedCommand::fromBinary(ByteBuffer *buff) {
	AbstractClientNotifyCommand::fromBinary(buff);

	AbstractTransferedData* d = TransactionTransferData::createFromBinary(buff); __STP(d);
	this->headerData = dynamic_cast<BlockHeaderTransferData*>(d);
	BinaryUtils::checkNotNull(this->headerData);

	__STP_MV(d);
}

ByteBuffer* ClientNotifyBlockMinedCommand::getSignBinary() const {
	BinaryUtils::checkNotNull(this->headerData);

	int total = this->headerData->binarySize();

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true);
	this->headerData->toBinary(buff);

	return buff;
}

IBlockObject* ClientNotifyBlockMinedCommand::copyData() const noexcept {
	return new ClientNotifyBlockMinedCommand(*this);
}

AbstractCommandResponse* ClientNotifyBlockMinedCommand::executeNotify(const PubSubId *pubsubId, ClientExecutor *clientExec) const {
	return clientExec->fireOnBlockMined(pubsubId, this->headerData);
}

void ClientNotifyBlockMinedCommand::setBlockHeaderTransferData(const BlockHeaderTransferData *data) {
	delete this->headerData;
	this->headerData = dynamic_cast<BlockHeaderTransferData*>(data->copyData());
}

} /* namespace codablecash */
