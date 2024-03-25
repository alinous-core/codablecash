/*
 * ClientNotifyNewTransactionCommand.cpp
 *
 *  Created on: 2023/10/05
 *      Author: iizuka
 */

#include "bc_p2p_cmd_client_notify/ClientNotifyNewTransactionCommand.h"
#include "bc_p2p_cmd_client_notify/ClientExecutor.h"

#include "pubsub_cmd/OkPubsubResponse.h"

#include "data_history_data/TransactionTransferData.h"

#include "bc_base/BinaryUtils.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"


namespace codablecash {

ClientNotifyNewTransactionCommand::ClientNotifyNewTransactionCommand(const ClientNotifyNewTransactionCommand &inst)
		:AbstractClientNotifyCommand(inst) {
	this->data = inst.data != nullptr ? new TransactionTransferData(*inst.data) : nullptr;
}

ClientNotifyNewTransactionCommand::ClientNotifyNewTransactionCommand()
	: AbstractClientNotifyCommand(TYPE_CLIENT_NOTIFY_NEW_TRANSACTION) {
	this->data = nullptr;
}

ClientNotifyNewTransactionCommand::~ClientNotifyNewTransactionCommand() {
	delete this->data;
}

int ClientNotifyNewTransactionCommand::binarySize() const {
	BinaryUtils::checkNotNull(this->data);

	int total = AbstractClientNotifyCommand::binarySize();
	total += this->data->binarySize();

	return total;
}

void ClientNotifyNewTransactionCommand::toBinary(ByteBuffer *buff) const {
	BinaryUtils::checkNotNull(this->data);

	AbstractClientNotifyCommand::toBinary(buff);
	this->data->toBinary(buff);

}

ByteBuffer* ClientNotifyNewTransactionCommand::getSignBinary() const {
	BinaryUtils::checkNotNull(this->data);

	int total = this->data->binarySize();

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true);
	this->data->toBinary(buff);

	return buff;
}

void ClientNotifyNewTransactionCommand::fromBinary(ByteBuffer *buff) {
	AbstractClientNotifyCommand::fromBinary(buff);

	AbstractTransferedData* d = TransactionTransferData::createFromBinary(buff); __STP(d);
	this->data = dynamic_cast<TransactionTransferData*>(d);
	BinaryUtils::checkNotNull(this->data);

	__STP_MV(d);
}

IBlockObject* ClientNotifyNewTransactionCommand::copyData() const noexcept {
	return new ClientNotifyNewTransactionCommand(*this);
}

AbstractCommandResponse* ClientNotifyNewTransactionCommand::executeNotify(const PubSubId *pubsubId, ClientExecutor *clientExec) const {
	return clientExec->fireOnNewTransaction(pubsubId, this->data);
}

void ClientNotifyNewTransactionCommand::setTransactionTransferData(const TransactionTransferData *data) noexcept {
	delete this->data;
	this->data = dynamic_cast<TransactionTransferData*>(data->copyData());
}

} /* namespace codablecash */
