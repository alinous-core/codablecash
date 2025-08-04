/*
 * RegisterVotePoolTransactionWalletHandler.cpp
 *
 *  Created on: 2023/04/15
 *      Author: iizuka
 */

#include "bc_wallet_trx/RegisterVotePoolTransactionWalletHandler.h"

#include "bc_base/BalanceUnit.h"
#include "bc_base/AddressDescriptor.h"

#include "bc_network/NodeIdentifier.h"
#include "bc_network/NodeIdentifierSource.h"

#include "bc_wallet_encoder/IWalletDataEncoder.h"

#include "bc_wallet_trx_repo/WalletAccountTrxRepository.h"

#include "bc_wallet/ReceivingAddressStore.h"
#include "bc_wallet/ChangeAddressStore.h"
#include "bc_wallet/WalletAccount.h"
#include "bc_wallet/HdWalleMuSigSignerProvidor.h"

#include "bc_wallet_trx_base/IUtxoCollector.h"
#include "bc_wallet_trx_base/HdWalletAccountTrxBuilderContext.h"
#include "bc_wallet_trx_base/ArrayUtxoFinder.h"

#include "base/StackRelease.h"

#include "bc_finalizer_trx/RegisterVotePoolTransaction.h"

#include "bc_trx_balance/BalanceUtxo.h"
#include "bc_trx_balance/BalanceShortageException.h"
#include "bc_trx_balance/BalanceUtxoReference.h"

#include "musig/IMuSigSigner.h"

#include "ecda/Secp256k1Point.h"

#include "bc/ExceptionThrower.h"


namespace codablecash {

RegisterVotePoolTransactionWalletHandler::RegisterVotePoolTransactionWalletHandler(WalletAccount* account)
		:AbstractWalletTransactionHandler(account) {

}

RegisterVotePoolTransactionWalletHandler::~RegisterVotePoolTransactionWalletHandler() {

}



RegisterVotePoolTransaction* RegisterVotePoolTransactionWalletHandler::createTransaction(
		const NodeIdentifierSource *source, const BalanceUnit& feeRate, const AddressDescriptor *addressDesc, const IWalletDataEncoder *encoder,
		ITransactionBuilderContext *context) {
	IUtxoCollector* collector = context->getUtxoCollector(); __STP(collector);
	ArrayUtxoFinder utxoFinder;

	HdWalleMuSigSignerProvidor* musigProvidor = context->getMusigSignProvidor(encoder); __STP(musigProvidor);

	NodeIdentifier nodeId = source->toNodeIdentifier();
	RegisterVotePoolTransaction* trx = new RegisterVotePoolTransaction(&nodeId, addressDesc); __STP(trx);

	trx->build();
	trx->sign(musigProvidor, &utxoFinder);
	trx->voterSign(source);

	{
		BalanceUnit amount(0L);
		collectUtxoRefs(trx, amount, feeRate, collector, &utxoFinder, musigProvidor, encoder);
	}

	trx->build();
	trx->sign(musigProvidor, &utxoFinder);
	trx->voterSign(source);

	return __STP_MV(trx);
}



void RegisterVotePoolTransactionWalletHandler::collectUtxoRefs(RegisterVotePoolTransaction *trx, const BalanceUnit& amount, const BalanceUnit& feeRate, IUtxoCollector *collector,
		ArrayUtxoFinder *utxoFinder, HdWalleMuSigSignerProvidor *musigProvidor, const IWalletDataEncoder *encoder) {
	int binSize = trx->binarySize();
	BalanceUnit fee = BalanceUnit(binSize) * feeRate;

	BalanceUnit required = fee + amount;
	BalanceUnit totalIn = utxoFinder->getTotalAmount();

	while(collector->hasNext() && totalIn.compareTo(&required) < 0){
		const BalanceUtxo* utxo = collector->next(); __STP(utxo);
		const AddressDescriptor* desc = utxo->getAddress();

		BalanceUtxoReference ref;
		ref.setUtxoId(utxo->getId(), desc);

		IMuSigSigner* signer = musigProvidor->getSigner(desc); __STP(signer);
		Secp256k1Point pt = signer->getxG();
		Secp256k1CompressedPoint xi = Secp256k1CompressedPoint::compress(&pt);
		ref.setXi(&xi);

		trx->addInputUtxoRef(&ref);

		utxoFinder->addUtxo(utxo);

		// calc addfee
		int refbinSize = ref.binarySize();
		binSize += refbinSize;

		fee = BalanceUnit(binSize) * feeRate;
		required = fee + amount;
		totalIn = utxoFinder->getTotalAmount();

		trx->setFeeAmount(&fee);

		// add exchange address
		if(totalIn.compareTo(&required) > 0){
			if(trx->getUtxoSize() == 0){
				// add
				ChangeAddressStore* changeAddresses = this->account->getChangeAddresses();
				AddressDescriptor* changeDesc = changeAddresses->getNextChangeAddress(encoder); __STP(changeDesc);

				int utxobinSize = 0;
				{
					BalanceUnit tmp(0L);
					BalanceUtxo utxo(tmp);
					utxo.setAddress(changeDesc);
					trx->addBalanceUtxo(&utxo);

					utxo.build();
					utxobinSize = utxo.binarySize();
				}
				binSize += utxobinSize;

				// recalc
				fee = BalanceUnit(binSize) * feeRate;

				required = fee + amount;
				totalIn = utxoFinder->getTotalAmount();

				// setup
				trx->setFeeAmount(&fee);

				BalanceUnit diff = totalIn - required;
				{
					AbstractUtxo* u = trx->getUtxo(0);
					BalanceUtxo* changeutxo = dynamic_cast<BalanceUtxo*>(u);
					changeutxo->setAmount(diff);
				}
			}
			else{
				BalanceUnit diff = totalIn - required;
				AbstractUtxo* u = trx->getUtxo(0);
				BalanceUtxo* changeutxo = dynamic_cast<BalanceUtxo*>(u);

				changeutxo->setAmount(diff);
			}
		}
	}

	ExceptionThrower<BalanceShortageException>::throwExceptionIfCondition(totalIn.compareTo(&required) < 0, L"Wallet don't have enough balance.", __FILE__, __LINE__);
}


void RegisterVotePoolTransactionWalletHandler::importTransaction(const AbstractBlockchainTransaction *__trx) {
	const RegisterVotePoolTransaction* trx = dynamic_cast<const RegisterVotePoolTransaction*>(__trx);
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
