/*
 * AbstractUtxoReference.cpp
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#include "bc_trx/AbstractUtxoReference.h"
#include "bc_trx/UtxoId.h"

#include "bc_trx_balance/BalanceUtxoReference.h"

#include "bc_finalizer_trx/TicketUtxoReference.h"
#include "bc_finalizer_trx/TicketVotedUtxoReference.h"

#include "bc_block_body/Coinbase.h"
#include "bc_block_body/Stakebase.h"

#include "bc_wallet_filter/BloomHash1024.h"

#include "bc_wallet_filter/BloomFilter1024.h"

namespace codablecash {

AbstractUtxoReference::AbstractUtxoReference(	const AbstractUtxoReference &inst) {
	this->utxoId = inst.utxoId != nullptr ? dynamic_cast<UtxoId*>(inst.utxoId->copyData()) : nullptr;
	this->bloomHash = inst.bloomHash != nullptr ? dynamic_cast<BloomHash1024*>(inst.bloomHash->copyData()) : nullptr;
}

AbstractUtxoReference::AbstractUtxoReference() {
	this->utxoId = nullptr;
	this->bloomHash = nullptr;
}

AbstractUtxoReference::~AbstractUtxoReference() {
	delete this->utxoId;
	delete this->bloomHash;
}

AbstractUtxoReference* AbstractUtxoReference::createFromBinary(ByteBuffer *in) {
	AbstractUtxoReference* ret = nullptr;

	uint8_t type = in->get();
	switch(type){
	case AbstractUtxoReference::UTXO_REF_TYPE_BALANCE:
		ret = new BalanceUtxoReference();
		break;
	case AbstractUtxoReference::UTXO_REF_TYPE_UTXO_TICKET:
		ret = new TicketUtxoReference();
		break;
	case AbstractUtxoReference::UTXO_REF_TYPE_UTXO_VOTED_TICKET:
		ret = new TicketVotedUtxoReference();
		break;
	case AbstractUtxoReference::UTXO_REF_TYPE_COINBASE:
		ret = new Coinbase();
		break;
	case AbstractUtxoReference::UTXO_REF_TYPE_STAKEBASE:
		ret = new Stakebase();
		break;
	default:
		return 0;
	}

	ret->fromBinary(in);

	return ret;
}

void AbstractUtxoReference::setUtxoId(const UtxoId *utxoId, const AddressDescriptor* addressDesc) noexcept {
	delete this->utxoId;
	this->utxoId = dynamic_cast<UtxoId*>(utxoId->copyData());

	delete this->bloomHash;
	this->bloomHash = nullptr;
	if(addressDesc != nullptr){
		BloomFilter1024 filter;
		this->bloomHash = filter.getHash(addressDesc);
	}
}

bool AbstractUtxoReference::checkFilter(const ArrayList<BloomFilter1024> *filtersList) const {
	bool ret = false;

	int maxLoop = filtersList->size();
	for(int i = 0; i != maxLoop; ++i){
		BloomFilter1024* filter = filtersList->get(i);

		bool bl = filter->checkBytes(this->bloomHash);
		if(bl){
			ret = true;
			break;
		}
	}

	return ret;
}

} /* namespace codablecash */
