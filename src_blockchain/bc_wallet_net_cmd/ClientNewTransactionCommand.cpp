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

#include "bc_wallet_net/NetworkWallet.h"

#include "bc_wallet_net_data/NetworkWalletData.h"

namespace codablecash {

ClientNewTransactionCommand::ClientNewTransactionCommand(const ClientNewTransactionCommand &inst)
		: AbstractClientQueueCommand(*this) {
	this->data = inst.data != nullptr ? dynamic_cast<TransactionTransferData*>(inst.data->copyData()) : nullptr;
}

ClientNewTransactionCommand::ClientNewTransactionCommand() : AbstractClientQueueCommand(AbstractClientQueueCommand::CLIENT_NEW_TRANSACTION) {
	this->data = nullptr;
}

ClientNewTransactionCommand::~ClientNewTransactionCommand() {
	delete this->data;
}

int ClientNewTransactionCommand::binarySize() const {
	BinaryUtils::checkNotNull(this->data);

	int total = AbstractClientQueueCommand::binarySize();

	total += this->data->binarySize();

	return total;
}

void ClientNewTransactionCommand::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->data);

	AbstractClientQueueCommand::toBinary(out);

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
	NetworkWalletData* walletData = wallet->getWalletData();
	walletData->addTransactionDataToMempool(this->data);
}

} /* namespace codablecash */
