/*
 * RegisterTicketTransactionWalletHandler.cpp
 *
 *  Created on: 2023/04/27
 *      Author: iizuka
 */

#include "bc_wallet_trx/RegisterTicketTransactionWalletHandler.h"

#include "bc_wallet_trx_repo/WalletAccountTrxRepository.h"

#include "bc_wallet_encoder/IWalletDataEncoder.h"

#include "bc_wallet/ReceivingAddressStore.h"
#include "bc_wallet/ChangeAddressStore.h"
#include "bc_wallet/WalletAccount.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_base/BalanceUnit.h"
#include "bc_base/AddressDescriptor.h"

#include "base/StackRelease.h"

#include "bc_finalizer_trx/RegisterTicketTransaction.h"

#include "bc_trx_balance/BalanceShortageException.h"
#include "bc_trx_balance/BalanceUtxo.h"
#include "bc_trx_balance/BalanceUtxoReference.h"

#include "musig/IMuSigSigner.h"

#include "ecda/Secp256k1Point.h"
#include "ecda/Secp256k1CompressedPoint.h"

#include "bc_wallet/HdWalleMuSigSignerProvidor.h"
namespace codablecash {

RegisterTicketTransactionWalletHandler::RegisterTicketTransactionWalletHandler(WalletAccount* account)
		:AbstractWalletTransactionHandler(account) {

}

RegisterTicketTransactionWalletHandler::~RegisterTicketTransactionWalletHandler() {

}

RegisterTicketTransaction* RegisterTicketTransactionWalletHandler::createTransaction(
		const NodeIdentifier *nodeId, const BalanceUnit stakeAmount, const BalanceUnit feeRate,
		const AddressDescriptor *addressDesc,
		const IWalletDataEncoder *encoder) {
	WalletAccountTrxRepository* trxRepo = this->account->getWalletAccountTrxRepository();
	ReceivingAddressStore* receivingAddresses = this->account->getReceivingAddresses();
	ChangeAddressStore* changeAddresses = this->account->getChangeAddresses();

	RegisterTicketTransaction* trx = new RegisterTicketTransaction(); __STP(trx);
	trx->setNodeId(nodeId);
	trx->setAddressDescriptor(addressDesc);
	trx->setAmounst(stakeAmount);

	ArrayList<BalanceUtxo> utxoList;
	utxoList.setDeleteOnExit();
	BalanceUnit remain = stakeAmount + (feeRate * BalanceUnit(137+4));

	remain = receivingAddresses->collectUtxos(trxRepo, &utxoList, remain, feeRate);
	if(!remain.isZero()){
		remain = changeAddresses->collectUtxos(trxRepo, &utxoList, remain, feeRate);
	}
	if(!remain.isZero()){
		throw new BalanceShortageException(__FILE__, __LINE__);
	}

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

	// uxto
	{
		BalanceUtxo utxo(balanceAll - stakeAmount);
		AddressDescriptor* dest = changeAddresses->getNextChangeAddress(encoder); __STP(dest);
		utxo.setAddress(dest);
		trx->addBalanceUtxo(&utxo);
	}
	trx->build();

	int binSize = trx->binarySize() + 65;
	BalanceUnit fee = BalanceUnit(binSize) * feeRate;
	trx->setFeeAmount(&fee);

	trx->build();

	// sign
	{
		HdWalleMuSigSignerProvidor providor(this->account, encoder);

		trx->sign(&providor, trxRepo);
	}

	return __STP_MV(trx);
}

void RegisterTicketTransactionWalletHandler::importTransaction(const AbstractBlockchainTransaction *__trx) {
	const RegisterTicketTransaction* trx = dynamic_cast<const RegisterTicketTransaction*>(__trx);
	WalletAccountTrxRepository* trxRepo = this->account->getWalletAccountTrxRepository();

	bool imported = false;

	// remove used utxos
	{
		int maxLoop = trx->getUtxoReferenceSize();
		for(int i = 0; i != maxLoop; ++i){
			AbstractUtxoReference* utxoRef = trx->getUtxoReference(i);
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
		if(this->account->hasAddress(addressDesc) && dynamic_cast<BalanceUtxo*>(utxo) != nullptr){
			trxRepo->importUtxo(dynamic_cast<BalanceUtxo*>(utxo));
			imported = true;
		}
	}

	if(imported){
		trxRepo->importTransaction(trx);
	}
}

} /* namespace codablecash */
