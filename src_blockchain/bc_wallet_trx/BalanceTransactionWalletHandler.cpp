/*
 * BalanceTransactionWalletHandler.cpp
 *
 *  Created on: 2023/02/25
 *      Author: iizuka
 */

#include "bc_wallet_trx/BalanceTransactionWalletHandler.h"

#include "bc_base/BalanceUnit.h"
#include "bc_base/BalanceAddress.h"
#include "bc_base/AddressDescriptor.h"

#include "base/ArrayList.h"
#include "base/StackRelease.h"

#include "bc_trx_balance/BalanceTransferTransaction.h"
#include "bc_trx_balance/BalanceUtxo.h"
#include "bc_trx_balance/BalanceUtxoReference.h"

#include "bc_wallet/WalletAccount.h"
#include "bc_wallet/AbstractAddressStore.h"
#include "bc_wallet/ReceivingAddressStore.h"
#include "bc_wallet/ChangeAddressStore.h"
#include "bc_wallet/HdWalleMuSigSignerProvidor.h"

#include "bc_wallet_trx_repo/WalletAccountTrxRepository.h"

#include "musig/IMuSigSigner.h"

#include "ecda/Secp256k1Point.h"
#include "ecda/Secp256k1CompressedPoint.h"

#include "bc_trx_balance/BalanceShortageException.h"

using namespace alinous;

namespace codablecash {

BalanceTransactionWalletHandler::BalanceTransactionWalletHandler(WalletAccount* account)
		:AbstractWalletTransactionHandler(account){

}

BalanceTransactionWalletHandler::~BalanceTransactionWalletHandler() {

}

BalanceTransferTransaction* BalanceTransactionWalletHandler::createTransaction(
		const AddressDescriptor *dest, const BalanceUnit amount,
		const BalanceUnit feeRate, bool feeIncluded,
		const IWalletDataEncoder *encoder) {

	WalletAccountTrxRepository* trxRepo = this->account->getWalletAccountTrxRepository();
	ReceivingAddressStore* receivingAddresses = this->account->getReceivingAddresses();
	ChangeAddressStore* changeAddresses = this->account->getChangeAddresses();

	BalanceTransferTransaction* trx = new BalanceTransferTransaction(); __STP(trx);

	ArrayList<BalanceUtxo> utxoList;
	utxoList.setDeleteOnExit();
	BalanceUnit remain = amount + (feeRate * BalanceUnit(162+4));
	remain = receivingAddresses->collectUtxos(trxRepo, &utxoList, remain, feeRate);
	if(!remain.isZero()){
		remain = changeAddresses->collectUtxos(trxRepo, &utxoList, remain, feeRate);
	}
	if(!remain.isZero()){
		throw new BalanceShortageException(__FILE__, __LINE__);
	}

	// add utxo ref
	BalanceUnit balanceAll(0L);
	{
		int maxLoop = utxoList.size();
		for(int i = 0; i != maxLoop; ++i){
			BalanceUtxo* utxo = utxoList.get(i);
			const AddressDescriptor* desc = utxo->getAddress();
			const BalanceAddress* addr = this->account->getAddress(desc);

			BalanceUtxoReference ref;
			ref.setUtxoId(utxo->getId());

			IMuSigSigner* signer = this->account->getSigner(desc, encoder); __STP(signer);
			Secp256k1Point pt = signer->getxG();
			Secp256k1CompressedPoint xi = Secp256k1CompressedPoint::compress(&pt);
			ref.setXi(&xi);

			trx->addInputUtxoRef(&ref);

			balanceAll += utxo->getAmount();
		}
	}

	// add uxto
	AddressDescriptor *skipfeedesc = nullptr;
	{
		BalanceUtxo utxo(amount);
		utxo.setAddress(dest);
		trx->addBalanceUtxo(&utxo);

		if(!feeIncluded){
			skipfeedesc = dynamic_cast<AddressDescriptor*>(utxo.getAddress()->copyData());
		}

		balanceAll -= amount;

		if(!balanceAll.isZero()){
			BalanceUtxo changeUtxo(balanceAll);
			AddressDescriptor* changeDesc = changeAddresses->getNextChangeAddress(encoder); __STP(changeDesc);
			changeUtxo.setAddress(changeDesc);
			trx->addBalanceUtxo(&changeUtxo);
		}
	}
	__STP(skipfeedesc);
	trx->build();

	// set fee
	int binSize = trx->binarySize() + 65;
	BalanceUnit fee = BalanceUnit(binSize) * feeRate;
	trx->setFeeAmount(&fee, skipfeedesc);

	trx->build();

	// sign
	{
		HdWalleMuSigSignerProvidor providor(this->account, encoder);

		trx->sign(&providor, trxRepo);
	}

	return __STP_MV(trx);

}

void BalanceTransactionWalletHandler::importTransaction(const AbstractBlockchainTransaction *__trx) {
	const BalanceTransferTransaction* trx = dynamic_cast<const BalanceTransferTransaction*>(__trx);
	WalletAccountTrxRepository* trxRepo = this->account->getWalletAccountTrxRepository();

	bool imported = false;

	// remove used utxos
	{
		const ArrayList<BalanceUtxoReference>* list = trx->getUtxoRefList();
		int maxLoop = list->size();
		for(int i = 0; i != maxLoop; ++i){
			BalanceUtxoReference* utxoRef = list->get(i);
			const UtxoId* utxoId = utxoRef->getUtxoId();

			BalanceUtxo* finded = trxRepo->getBalanceUtxo(utxoId); __STP(finded);
			if(finded != nullptr){
				trxRepo->removeUtxo(utxoId);
				imported = true;
			}
		}
	}

	// add utxo
	int maxLoop = trx->getUtxoSize();
	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxo* utxo = trx->getUtxo(i);

		const AddressDescriptor* addressDesc = utxo->getAddress();
		if(this->account->hasAddress(addressDesc)){
			trxRepo->importUtxo(dynamic_cast<BalanceUtxo*>(utxo));
			imported = true;
		}
	}

	if(imported){
		trxRepo->importTransaction(trx);
	}
}

} /* namespace codablecash */
