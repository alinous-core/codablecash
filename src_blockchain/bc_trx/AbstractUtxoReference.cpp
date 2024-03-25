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

namespace codablecash {

AbstractUtxoReference::AbstractUtxoReference(	const AbstractUtxoReference &inst) {
	this->utxoId = inst.utxoId != nullptr ? dynamic_cast<UtxoId*>(inst.utxoId->copyData()) : nullptr;
}

AbstractUtxoReference::AbstractUtxoReference() {
	this->utxoId = nullptr;
}

AbstractUtxoReference::~AbstractUtxoReference() {
	delete this->utxoId;
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

void AbstractUtxoReference::setUtxoId(const UtxoId *utxoId) noexcept {
	delete this->utxoId;
	this->utxoId = dynamic_cast<UtxoId*>(utxoId->copyData());
}

} /* namespace codablecash */
