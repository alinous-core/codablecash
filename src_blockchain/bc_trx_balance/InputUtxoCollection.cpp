/*
 * InputUtxoCollection.cpp
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#include "bc_trx_balance/InputUtxoCollection.h"
#include "bc_trx_balance/BalanceUtxoReference.h"
#include "bc_trx_balance/IMuSigSignerProvidor.h"
#include "bc_trx_balance/BalanceTransferTransaction.h"
#include "bc_trx_balance/IUtxoFinder.h"
#include "bc_trx_balance/BalanceUtxo.h"

#include "musig/MuSigBuilder.h"

#include "base/StackRelease.h"

#include "bc_base/AddressDescriptor.h"
#include "bc_base/BalanceAddress.h"
#include "bc_base/BinaryUtils.h"

#include "musig/MuSig.h"


namespace codablecash {

InputUtxoCollection::InputUtxoCollection(const InputUtxoCollection &inst) {
	int maxLoop = inst.list.size();
	for(int i = 0; i != maxLoop; ++i){
		BalanceUtxoReference* ref = inst.list.get(i);

		addReference(ref);
	}
}

InputUtxoCollection::InputUtxoCollection() {

}

InputUtxoCollection::~InputUtxoCollection() {
	this->list.deleteElements();
}

void InputUtxoCollection::addReference(const BalanceUtxoReference *ref) noexcept {
	BalanceUtxoReference* r  = dynamic_cast<BalanceUtxoReference*>(ref->copyData());
	this->list.addElement(r);
}

MuSig InputUtxoCollection::sign(IMuSigSignerProvidor* providor, IUtxoFinder* finder, const char *data, int length) const {
	MuSigBuilder builder;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		const BalanceUtxoReference* ref = this->list.get(i);
		const UtxoId* utxoId = ref->getUtxoId();

		BalanceUtxo* utxo = finder->getBalanceUtxo(utxoId); __STP(utxo);
		const AddressDescriptor* desc = utxo->getAddress();

		IMuSigSigner* signer = providor->getSigner(desc);
		builder.addSigner(signer);
	}

	MuSig sig = builder.sign(data, length);
	return sig;
}

int InputUtxoCollection::binarySize() const {
	int total = sizeof(uint16_t);

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		BalanceUtxoReference* ref = this->list.get(i);

		total += ref->binarySize();
	}

	return total;
}

void InputUtxoCollection::toBinary(ByteBuffer *out) const {
	int maxLoop = this->list.size();
	out->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		BalanceUtxoReference* ref = this->list.get(i);

		ref->toBinary(out);
	}
}

InputUtxoCollection* InputUtxoCollection::fromBinary(ByteBuffer *in) {
	InputUtxoCollection* collection = new InputUtxoCollection(); __STP(collection);

	int maxLoop = in->getShort();
	BinaryUtils::checkUShortRange(maxLoop, 0, BalanceTransferTransaction::MAX_INPUT_ELEMENT);

	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxoReference* __ref = AbstractUtxoReference::createFromBinary(in); __STP(__ref);
		BinaryUtils::checkUint8Value(__ref->getType(), BalanceUtxoReference::UTXO_REF_TYPE_BALANCE);

		BalanceUtxoReference* ref = dynamic_cast<BalanceUtxoReference*>(__ref);
		collection->addReference(ref);
	}

	return __STP_MV(collection);
}

} /* namespace codablecash */
