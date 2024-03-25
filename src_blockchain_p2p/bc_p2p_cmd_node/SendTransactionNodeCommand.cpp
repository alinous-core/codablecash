/*
 * SendTransactionNodeCommand.cpp
 *
 *  Created on: 2023/10/01
 *      Author: iizuka
 */

#include "bc_p2p_cmd_node/SendTransactionNodeCommand.h"
#include "bc_p2p_cmd_node/InvalidTransactionException.h"

#include "data_history_data/TransactionTransferData.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

#include "bc/CodablecashNodeInstance.h"
#include "bc/ExceptionThrower.h"

#include "bc_p2p/BlockchainNodeHandshake.h"
#include "bc_p2p/BlochchainP2pManager.h"

#include "bc_p2p_processor/P2pRequestProcessor.h"

#include "bc_memorypool/MemPoolTransaction.h"
#include "bc_memorypool/MemoryPool.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc_status_cache_context/IStatusCacheContext.h"

#include "bc_trx/AbstractBlockchainTransaction.h"

#include "pubsub_cmd/OkPubsubResponse.h"

#include "command_queue_cmd/ClientTransactionAcceptionQueueCommand.h"
#include "command_queue_cmd/NodeTransactionAcceptionQueueCommand.h"

#include "bc_p2p_cmd_client_notify/ClientNotifyNewTransactionCommand.h"


namespace codablecash {

SendTransactionNodeCommand::SendTransactionNodeCommand(const SendTransactionNodeCommand &inst)
		: AbstractNodeCommand(inst) {
	this->data = inst.data != nullptr ? dynamic_cast<TransactionTransferData*>(inst.data->copyData()) : nullptr;
}

SendTransactionNodeCommand::SendTransactionNodeCommand() : AbstractNodeCommand(TYPE_NODE_SEND_TRANSACTION) {
	this->data = nullptr;
}

SendTransactionNodeCommand::~SendTransactionNodeCommand() {
	delete this->data;
}

int SendTransactionNodeCommand::binarySize() const {
	BinaryUtils::checkNotNull(this->data);

	int total = AbstractNodeCommand::binarySize();

	total += this->data->binarySize();

	return total;
}

void SendTransactionNodeCommand::toBinary(ByteBuffer *buff) const {
	BinaryUtils::checkNotNull(this->data);

	AbstractNodeCommand::toBinary(buff);
	this->data->toBinary(buff);
}

ByteBuffer* SendTransactionNodeCommand::getSignBinary() const {
	BinaryUtils::checkNotNull(this->data);

	int total = this->data->binarySize();

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true);
	this->data->toBinary(buff);

	return buff;
}

void SendTransactionNodeCommand::fromBinary(ByteBuffer *buff) {
	AbstractNodeCommand::fromBinary(buff);

	AbstractTransferedData* d = TransactionTransferData::createFromBinary(buff); __STP(d);
	this->data = dynamic_cast<TransactionTransferData*>(d);
	BinaryUtils::checkNotNull(this->data);

	__STP_MV(d);
}

IBlockObject* SendTransactionNodeCommand::copyData() const noexcept {
	return new SendTransactionNodeCommand(*this);
}

void SendTransactionNodeCommand::setTransactionTransferData(const TransactionTransferData *data) noexcept {
	delete this->data;
	this->data = dynamic_cast<TransactionTransferData*>(data->copyData());
}

AbstractCommandResponse* SendTransactionNodeCommand::executeAsNode(BlockchainNodeHandshake *nodeHandShake, CodablecashNodeInstance *inst, bool suspend) const {
	P2pRequestProcessor* processor = inst->getP2pRequestProcessor();

	uint16_t zoneSelf = inst->getZoneSelf();
	bool alreadyHas = processor->hasHistory(this->data);
	if(!alreadyHas){
		BlockchainController* ctrl = inst->getController();
		MemoryPool* memPool = inst->getMemoryPool();

		processor->addHistory(this->data);

		{
			bool res = processTransaction(zoneSelf, memPool, ctrl);
			ExceptionThrower<InvalidTransactionException>::throwExceptionIfCondition(!res, L"Invalid Transaction was sent.", __FILE__, __LINE__);
		}

		// put on queue
		{
			const PubSubId* pubsubId = nodeHandShake->getPubsubId();

			NodeTransactionAcceptionQueueCommand cmd;
			cmd.setData(this->data);

			processor->putQueue(pubsubId, &cmd);
		}

		if(!suspend){
			// bload cast
			BlochchainP2pManager* manager = inst->getBlochchainP2pManager();
			{
				SendTransactionNodeCommand command;
				command.setTransactionTransferData(this->data);

				command.sign(inst->getNetworkKey());

				const NodeIdentifier* nodeId = nodeHandShake->getNodeId(); // exclude source handshake

				manager->bloadCastWithinZone(zoneSelf, nodeId, &command, processor);
			}
			{
				ClientNotifyNewTransactionCommand command;
				command.setTransactionTransferData(this->data);
				command.sign(inst->getNetworkKey());

				manager->bloadCastToClients(&command, processor);
			}
		}
	}

	return new OkPubsubResponse();
}

bool SendTransactionNodeCommand::processTransaction(uint16_t zoneSelf, MemoryPool *memPool, BlockchainController *ctrl) const {
	AbstractBlockchainTransaction* trx = this->data->getTransaction();

	MemPoolTransaction* memTrx = memPool->begin(); __STP(memTrx);

	IStatusCacheContext* context = ctrl->getStatusCacheContext(zoneSelf); __STP(context);

	return trx->validateOnAccept(memTrx, context);
}

} /* namespace codablecash */
