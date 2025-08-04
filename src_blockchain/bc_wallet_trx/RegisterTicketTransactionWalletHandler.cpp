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

#include "bc_wallet_trx_base/IUtxoCollector.h"
#include "bc_wallet_trx_base/HdWalletAccountTrxBuilderContext.h"
#include "bc_wallet_trx_base/ArrayUtxoFinder.h"

#include "bc/ExceptionThrower.h"

namespace codablecash {

RegisterTicketTransactionWalletHandler::RegisterTicketTransactionWalletHandler(WalletAccount* account)
		:AbstractWalletTransactionHandler(account) {

}

RegisterTicketTransactionWalletHandler::~RegisterTicketTransactionWalletHandler() {

}

RegisterTicketTransaction* RegisterTicketTransactionWalletHandler::createTransaction(const NodeIdentifier *nodeId, const BalanceUnit& stakeAmount,
		const BalanceUnit& feeRate, const AddressDescriptor *ticketReturnaddressDesc, const IWalletDataEncoder *encoder, ITransactionBuilderContext *context) {
	IUtxoCollector* collector = context->getUtxoCollector(); __STP(collector);
	ArrayUtxoFinder utxoFinder;

	HdWalleMuSigSignerProvidor* musigProvidor = context->getMusigSignProvidor(encoder); __STP(musigProvidor);

	RegisterTicketTransaction* trx = new RegisterTicketTransaction(); __STP(trx);
	trx->setNodeId(nodeId);
	trx->setAddressDescriptor(ticketReturnaddressDesc);
	trx->setAmounst(stakeAmount);

	trx->build();
	trx->sign(musigProvidor, &utxoFinder);

	{
		BalanceUnit amount(stakeAmount);
		collectUtxoRefs(trx, amount, feeRate, collector, &utxoFinder, musigProvidor, encoder);
	}

	trx->build();
	trx->sign(musigProvidor, &utxoFinder);

	return __STP_MV(trx);
}

void RegisterTicketTransactionWalletHandler::collectUtxoRefs(RegisterTicketTransaction *trx, BalanceUnit& amount,
		const BalanceUnit& feeRate, IUtxoCollector *collector, ArrayUtxoFinder *utxoFinder, HdWalleMuSigSignerProvidor *musigProvidor, const IWalletDataEncoder *encoder) {
	int binSize = trx->binarySize();
	BalanceUnit fee = BalanceUnit(binSize) * feeRate;

	BalanceUnit required = fee + amount;
	BalanceUnit totalIn = utxoFinder->getTotalAmount();

	while(collector->hasNext() && totalIn.compareTo(&required) < 0){
		BalanceUtxo* utxo = collector->next(); __STP(utxo);
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
			if(trx->getUtxoSize() == 1){
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
					BalanceUtxo* utxo = dynamic_cast<BalanceUtxo*>(u);
					utxo->setAmount(diff);
				}
			}
			else {
				BalanceUnit diff = totalIn - required;
				AbstractUtxo* u = trx->getUtxo(0);
				BalanceUtxo* utxo = dynamic_cast<BalanceUtxo*>(u);

				utxo->setAmount(diff);
			}
		}
	}

	ExceptionThrower<BalanceShortageException>::throwExceptionIfCondition(totalIn.compareTo(&required) < 0, L"Wallet don't have enough balance.", __FILE__, __LINE__);
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
