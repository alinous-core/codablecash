/*
 * ClientTransactionAcceptionQueueCommand.cpp
 *
 *  Created on: 2023/09/29
 *      Author: iizuka
 */

#include "command_queue_cmd/ClientTransactionAcceptionQueueCommand.h"

#include "data_history_data/TransactionTransferData.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"
namespace codablecash {

ClientTransactionAcceptionQueueCommand::ClientTransactionAcceptionQueueCommand(const ClientTransactionAcceptionQueueCommand &inst)
		: AbstractTransactionAcceptionQueueCommand(inst){

}

ClientTransactionAcceptionQueueCommand::ClientTransactionAcceptionQueueCommand()
	: AbstractTransactionAcceptionQueueCommand(TYPE_NODE_CLIENT_TRANSACTION_ACCEPT_QUEUE) {

}

ClientTransactionAcceptionQueueCommand::~ClientTransactionAcceptionQueueCommand() {

}

IBlockObject* ClientTransactionAcceptionQueueCommand::copyData() const noexcept {
	return new ClientTransactionAcceptionQueueCommand(*this);
}


} /* namespace codablecash */
