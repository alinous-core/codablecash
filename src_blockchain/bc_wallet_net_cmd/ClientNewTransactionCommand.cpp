/*
 * ClientNewTransactionCommand.cpp
 *
 *  Created on: Apr 15, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_cmd/ClientNewTransactionCommand.h"

#include "data_history_data/TransactionTransferData.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

namespace codablecash {

ClientNewTransactionCommand::ClientNewTransactionCommand(const ClientNewTransactionCommand &inst)
		: AbstractClientCommand(*this) {
	this->data = inst.data != nullptr ? dynamic_cast<TransactionTransferData*>(inst.data->copyData()) : nullptr;
}

ClientNewTransactionCommand::ClientNewTransactionCommand() : AbstractClientCommand(AbstractClientCommand::CLIENT_NEW_TRANSACTION) {
	this->data = nullptr;
}

ClientNewTransactionCommand::~ClientNewTransactionCommand() {
	delete this->data;
}

int ClientNewTransactionCommand::binarySize() const {
	BinaryUtils::checkNotNull(this->data);

	int total = AbstractClientCommand::binarySize();

	total += this->data->binarySize();

	return total;
}

void ClientNewTransactionCommand::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->data);

	AbstractClientCommand::toBinary(out);

	this->data->toBinary(out);
}

void ClientNewTransactionCommand::fromBinary(ByteBuffer *in) {
	AbstractTransferedData* obj = AbstractTransferedData::createFromBinary(in); __STP(obj);

	TransactionTransferData* d = dynamic_cast<TransactionTransferData*>(obj);
	BinaryUtils::checkNotNull(d);

	__STP_MV(obj);
	this->data = d;
}

IBlockObject* ClientNewTransactionCommand::copyData() const noexcept {
	return new ClientNewTransactionCommand(*this);
}

void ClientNewTransactionCommand::setData(const TransactionTransferData *d) {
	delete this->data;
	this->data = dynamic_cast<TransactionTransferData*>(d->copyData());
}

void ClientNewTransactionCommand::process(NetworkWallet *wallet) const {
	// TODO ClientNewTransactionCommand
}

} /* namespace codablecash */
