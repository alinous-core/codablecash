/*
 * SendTransactionClientCommand.cpp
 *
 *  Created on: 2023/09/24
 *      Author: iizuka
 */

#include "bc_p2p_cmd_client/SendTransactionClientCommand.h"
#include "bc_p2p_cmd_client/AbstractClientRequestCommand.h"

#include "data_history_data/TransactionTransferData.h"
#include "data_history/AbstractTransferedData.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"
#include "base/Exception.h"

#include "bc/CodablecashNodeInstance.h"

#include "bc_p2p_processor/P2pRequestProcessor.h"

#include "pubsub_cmd/OkPubsubResponse.h"
#include "pubsub_cmd/ErrorPubsubResponse.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc_status_cache_context/IStatusCacheContext.h"

#include "bc_trx/AbstractBlockchainTransaction.h"

#include "bc_memorypool/MemoryPool.h"
#include "bc_memorypool/MemPoolTransaction.h"

#include "command_queue_cmd/ClientTransactionAcceptionQueueCommand.h"

#include "bc_p2p/ClientNodeHandshake.h"
#include "bc_p2p/BlochchainP2pManager.h"

#include "bc_p2p_cmd_client_notify/ClientNotifyNewTransactionCommand.h"

#include "bc_p2p_cmd_node_consensus/SendTransactionNodeCommand.h"

namespace codablecash {

SendTransactionClientCommand::SendTransactionClientCommand(const SendTransactionClientCommand &inst) : AbstractClientRequestCommand(inst) {
	this->data = inst.data != nullptr ? new TransactionTransferData(*inst.data) : nullptr;
}

SendTransactionClientCommand::SendTransactionClientCommand() : AbstractClientRequestCommand(TYPE_CLIENT_TRANSACTION_TRANSFER) {
	this->data = nullptr;
}

SendTransactionClientCommand::~SendTransactionClientCommand() {
	delete this->data;
}

int SendTransactionClientCommand::binarySize() const {
	BinaryUtils::checkNotNull(this->data);

	int total = AbstractClientRequestCommand::binarySize();
	total += this->data->binarySize();

	return total;
}

ByteBuffer* SendTransactionClientCommand::getSignBinary() const {
	BinaryUtils::checkNotNull(this->data);

	int total = this->data->binarySize();

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true);
	this->data->toBinary(buff);

	return buff;
}

void SendTransactionClientCommand::toBinary(ByteBuffer *buff) const {
	BinaryUtils::checkNotNull(this->data);

	AbstractClientRequestCommand::toBinary(buff);
	this->data->toBinary(buff);
}

void SendTransactionClientCommand::fromBinary(ByteBuffer *buff) {
	AbstractClientRequestCommand::fromBinary(buff);

	AbstractTransferedData* d = TransactionTransferData::createFromBinary(buff); __STP(d);
	this->data = dynamic_cast<TransactionTransferData*>(d);
	BinaryUtils::checkNotNull(this->data);

	__STP_MV(d);
}

IBlockObject* SendTransactionClientCommand::copyData() const noexcept {
	return new SendTransactionClientCommand(*this);
}

AbstractCommandResponse* SendTransactionClientCommand::executeAsClient(ClientNodeHandshake *clientHandshake, CodablecashNodeInstance *inst) const {
	P2pRequestProcessor* processor = inst->getP2pRequestProcessor();

	uint16_t zoneSelf = inst->getZoneSelf();

	bool alreadyHas = processor->hasHistory(this->data);
	if(!alreadyHas){
		BlockchainController* ctrl = inst->getController();
		MemoryPool* memPool = inst->getMemoryPool();

		processor->addHistory(this->data);

		if(!processTransaction(zoneSelf, memPool, ctrl)){
			UnicodeString errstr(L"Invalid Transaction.");
			ErrorPubsubResponse* response = new ErrorPubsubResponse();
			response->setMessage(&errstr);

			return response;
		}

		// put on queue
		{
			const PubSubId* pubsubId = clientHandshake->getPubsubId();

			ClientTransactionAcceptionQueueCommand cmd; // put into memory pool
			cmd.setData(this->data);

			processor->putQueue(pubsubId, &cmd);
		}

		// bload cast
		BlochchainP2pManager* manager = inst->getBlochchainP2pManager();
		{
			SendTransactionNodeCommand command;
			command.setTransactionTransferData(this->data);
			command.sign(inst->getNetworkKey());

			manager->bloadCastWithinZone(zoneSelf, &command, processor);
		}
		{
			ClientNotifyNewTransactionCommand command;
			command.setTransactionTransferData(this->data);
			command.sign(inst->getNetworkKey());

			manager->bloadCastToClients(&command, processor);
		}
	}

	return new OkPubsubResponse();
}

bool SendTransactionClientCommand::processTransaction(uint16_t zoneSelf, MemoryPool* memPool, BlockchainController* ctrl) const {
	AbstractBlockchainTransaction* trx = this->data->getTransaction();

	MemPoolTransaction* memTrx = memPool->begin(); __STP(memTrx);

	IStatusCacheContext* context = ctrl->getStatusCacheContext(zoneSelf); __STP(context);

	return trx->validateOnAccept(memTrx, context);
}

void SendTransactionClientCommand::setTransaction(const AbstractBlockchainTransaction *trx) {
	delete this->data;
	this->data = new TransactionTransferData();
	this->data->setTransaction(trx);
}

} /* namespace codablecash */
