/*
 * GenesisTransactionHandler.cpp
 *
 *  Created on: 2023/02/25
 *      Author: iizuka
 */

#include "bc_wallet_trx/GenesisTransactionHandler.h"

#include "bc_base/AddressDescriptor.h"
#include "bc_base/BalanceUnit.h"

#include "bc_wallet/WalletAccount.h"

#include "base/StackRelease.h"

#include "bc_trx_genesis/GenesisTransaction.h"

#include "bc_trx_balance/BalanceUtxo.h"

#include "bc_trx/AbstractBlockchainTransaction.h"

#include "bc_wallet_trx_repo/WalletAccountTrxRepository.h"
namespace codablecash {

GenesisTransactionHandler::GenesisTransactionHandler(WalletAccount* account)
			: AbstractWalletTransactionHandler(account) {

}

GenesisTransactionHandler::~GenesisTransactionHandler() {

}

GenesisTransaction* GenesisTransactionHandler::createTransaction(const BalanceUnit amount, int pos) {
	AddressDescriptor* addressDesc = this->account->getReceivingAddressDescriptor(pos); __STP(addressDesc);
	GenesisTransaction* trx = new GenesisTransaction(); __STP(trx);

	BalanceUtxo utxo(amount);
	utxo.setAddress(addressDesc);

	trx->addBalanceUtxo(&utxo);
	trx->build();

	return __STP_MV(trx);
}

void GenesisTransactionHandler::importTransaction(const AbstractBlockchainTransaction *__trx) {
	const GenesisTransaction* trx = dynamic_cast<const GenesisTransaction*>(__trx);
	WalletAccountTrxRepository* trxRepo = this->account->getWalletAccountTrxRepository();

	bool imported = false;

	int maxLoop = trx->getUtxoSize();
	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxo* utxo = trx->getUtxo(i);

		const AddressDescriptor* addressDesc = utxo->getAddress();
		if(this->account->hasAddress(addressDesc)){
			trxRepo->importUtxo(utxo);
			imported = true;
		}
	}

	if(imported){
		trxRepo->importTransaction(trx);
	}
}

} /* namespace codablecash */
