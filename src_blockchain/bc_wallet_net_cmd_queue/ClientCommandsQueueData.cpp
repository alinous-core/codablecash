/*
 * ClientCommandsQueueData.cpp
 *
 *  Created on: Mar 26, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_cmd_queue/ClientCommandsQueueData.h"

#include "bc_wallet_net_cmd/ClientNewTransactionCommand.h"

#include "base/StackRelease.h"

#include "data_history/AbstractTransferedData.h"

#include "bc/ExceptionThrower.h"

#include "data_history_data/TransactionTransferData.h"

namespace codablecash {

ClientCommandsQueueData::ClientCommandsQueueData(const ClientCommandsQueueData &inst) {
	this->data = inst.data != nullptr ? dynamic_cast<AbstractTransferedData*>(inst.data->copyData()) : nullptr;
}

ClientCommandsQueueData::ClientCommandsQueueData() {
	this->data = nullptr;
}

ClientCommandsQueueData::~ClientCommandsQueueData() {
	delete this->data;
}

int ClientCommandsQueueData::binarySize() const {
	int total = 0;

	total += sizeof(uint8_t);

	if(this->data != nullptr){
		total += this->data->binarySize();
	}

	return total;
}

void ClientCommandsQueueData::toBinary(ByteBuffer *out) const {
	uint8_t bl = this->data != nullptr ? 1 : 0;
	out->put(bl);

	if(this->data != nullptr){
		this->data->toBinary(out);
	}
}

ClientCommandsQueueData* ClientCommandsQueueData::createFromBinary(ByteBuffer *in) {
	ClientCommandsQueueData* data = new ClientCommandsQueueData(); __STP(data);

	uint8_t bl = in->get();
	if(bl > 0){
		AbstractTransferedData* tdata = AbstractTransferedData::createFromBinary(in);
		data->data = tdata;
	}

	return __STP_MV(data);
}

IBlockObject* ClientCommandsQueueData::copyData() const noexcept {
	return new ClientCommandsQueueData(*this);
}

void ClientCommandsQueueData::setData(const AbstractTransferedData *dt) {
	delete this->data;
	this->data = dynamic_cast<AbstractTransferedData*>(dt->copyData());
}

AbstractClientCommand* ClientCommandsQueueData::toClientCommand() const {
	AbstractClientCommand* cmd = nullptr;

	if(this->data != nullptr){
		uint8_t type = this->data->getType();
		if(AbstractTransferedData::DATA_TRANSACTION == type){
			ClientNewTransactionCommand* clcmd = new ClientNewTransactionCommand();
			clcmd->setData(dynamic_cast<TransactionTransferData*>(this->data));

			cmd = clcmd;
		}
	}

	return cmd;
}

} /* namespace codablecash */
