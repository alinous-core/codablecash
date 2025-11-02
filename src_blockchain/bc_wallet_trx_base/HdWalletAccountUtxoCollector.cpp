/*
 * HdWalletAccountUtxoCollector.cpp
 *
 *  Created on: Jun 4, 2025
 *      Author: iizuka
 */

#include "bc_wallet_trx_base/HdWalletAccountUtxoCollector.h"

#include "bc_wallet/WalletAccount.h"
#include "bc_wallet/ReceivingAddressStore.h"
#include "bc_wallet/ChangeAddressStore.h"

#include "bc_wallet_trx_repo/WalletAccountTrxRepository.h"

#include "bc_trx_balance/BalanceUtxo.h"

#include "base/StackRelease.h"

#include "bc_base/BalanceAddress.h"
#include "bc_base/AddressDescriptor.h"


namespace codablecash {

HdWalletAccountUtxoCollector::HdWalletAccountUtxoCollector(WalletAccount* account) {
	this->trxRepo = account->getWalletAccountTrxRepository();
	this->receivingAddresses = account->getReceivingAddresses();
	this->changeAddresses = account->getChangeAddresses();

	this->changeaddr = false;
	this->cursor = 0;
	this->butxoList = new ArrayList<BalanceUtxo>();

	importReceivingUtxos();
}

HdWalletAccountUtxoCollector::~HdWalletAccountUtxoCollector() {
	this->trxRepo = nullptr;
	this->receivingAddresses = nullptr;
	this->changeAddresses = nullptr;

	this->butxoList->deleteElements();
	delete this->butxoList;
}

bool HdWalletAccountUtxoCollector::hasNext() {
	if(!this->changeaddr){
		if(this->butxoList->size() > this->cursor){
			return true;
		}

		this->changeaddr = true;
		this->cursor = 0;
		importChangeUtxos();
	}

	return this->butxoList->size() > this->cursor;
}

BalanceUtxo* HdWalletAccountUtxoCollector::next() {
	const BalanceUtxo* utxo = this->butxoList->get(this->cursor);
	this->cursor++;

	return dynamic_cast<BalanceUtxo*>(utxo->copyData());
}

void HdWalletAccountUtxoCollector::importReceivingUtxos() {
	this->butxoList->deleteElements();
	this->butxoList->reset();

	int maxLoop = this->receivingAddresses->size();
	for(int i = 0; i != maxLoop; ++i){
		const BalanceAddress* addr = this->receivingAddresses->getAddress(i);
		AddressDescriptor* desc = addr->toAddressDescriptor(); __STP(desc);

		ArrayList<BalanceUtxo>* list = this->trxRepo->getBalanceUtxos(desc); __STP(list);
		if(list != nullptr){
			this->butxoList->addAll(list);
		}
	}

}

void HdWalletAccountUtxoCollector::importChangeUtxos() {
	this->butxoList->deleteElements();
	this->butxoList->reset();

	int maxLoop = this->changeAddresses->size();
	for(int i = 0; i != maxLoop; ++i){
		const BalanceAddress* addr = this->changeAddresses->getAddress(i);
		AddressDescriptor* desc = addr->toAddressDescriptor(); __STP(desc);

		ArrayList<BalanceUtxo>* list = this->trxRepo->getBalanceUtxos(desc); __STP(list);
		if(list != nullptr){
			this->butxoList->addAll(list);
		}
	}
}

} /* namespace codablecash */
