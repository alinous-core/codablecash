/*
 * ClientCommandsQueueData.cpp
 *
 *  Created on: Mar 26, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_cmd_queue/ClientCommandsQueueData.h"

#include "bc_wallet_net_cmd/ClientNewTransactionCommand.h"
#include "bc_wallet_net_cmd/ClientBlockMinedCommand.h"

#include "base/StackRelease.h"

#include "data_history/AbstractTransferedData.h"

#include "data_history_data/TransactionTransferData.h"

#include "bc/ExceptionThrower.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_base/BinaryUtils.h"

namespace codablecash {

ClientCommandsQueueData::ClientCommandsQueueData(const ClientCommandsQueueData &inst) {
	this->data = inst.data != nullptr ? dynamic_cast<AbstractTransferedData*>(inst.data->copyData()) : nullptr;
	this->sourceNodeId = inst.sourceNodeId != nullptr ? dynamic_cast<NodeIdentifier*>(inst.sourceNodeId->copyData()) : nullptr;
}

ClientCommandsQueueData::ClientCommandsQueueData() {
	this->data = nullptr;
	this->sourceNodeId = nullptr;
}

ClientCommandsQueueData::~ClientCommandsQueueData() {
	delete this->data;
	delete this->sourceNodeId;
}

int ClientCommandsQueueData::binarySize() const {
	BinaryUtils::checkNotNull(this->sourceNodeId);

	int total = 0;

	total += sizeof(uint8_t);

	if(this->data != nullptr){
		total += this->data->binarySize();
	}

	total += this->sourceNodeId->binarySize();

	return total;
}

void ClientCommandsQueueData::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->sourceNodeId);

	uint8_t bl = this->data != nullptr ? 1 : 0;
	out->put(bl);

	if(this->data != nullptr){
		this->data->toBinary(out);
	}

	this->sourceNodeId->toBinary(out);
}

ClientCommandsQueueData* ClientCommandsQueueData::createFromBinary(ByteBuffer *in) {
	ClientCommandsQueueData* data = new ClientCommandsQueueData(); __STP(data);

	uint8_t bl = in->get();
	if(bl > 0){
		AbstractTransferedData* tdata = AbstractTransferedData::createFromBinary(in);
		data->data = tdata;
	}

	data->sourceNodeId = NodeIdentifier::fromBinary(in);

	return __STP_MV(data);
}

IBlockObject* ClientCommandsQueueData::copyData() const noexcept {
	return new ClientCommandsQueueData(*this);
}

void ClientCommandsQueueData::setData(const AbstractTransferedData *dt) {
	delete this->data;
	this->data = dynamic_cast<AbstractTransferedData*>(dt->copyData());
}

AbstractClientQueueCommand* ClientCommandsQueueData::toClientCommand() const {
	AbstractClientQueueCommand* cmd = nullptr;

	if(this->data != nullptr){
		uint8_t type = this->data->getType();
		if(AbstractTransferedData::DATA_TRANSACTION == type){
			ClientNewTransactionCommand* clcmd = new ClientNewTransactionCommand();
			clcmd->setData(dynamic_cast<TransactionTransferData*>(this->data));

			cmd = clcmd;
		}
		else if(AbstractTransferedData::DATA_BLOCKHEADER == type){
			ClientBlockMinedCommand* clcmd = new ClientBlockMinedCommand();
			clcmd->setData(dynamic_cast<BlockHeaderTransferData*>(this->data));
			clcmd->setSourceNodeId(this->sourceNodeId);

			cmd = clcmd;
		}
	}

	return cmd;
}

void ClientCommandsQueueData::setSourceNodeId(const NodeIdentifier *nodeId) {
	this->sourceNodeId = dynamic_cast<NodeIdentifier*>(nodeId->copyData());
}

} /* namespace codablecash */
