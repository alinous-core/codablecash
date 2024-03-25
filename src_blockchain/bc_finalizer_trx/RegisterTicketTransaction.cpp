/*
 * RegisterTicketTransaction.cpp
 *
 *  Created on: 2023/04/15
 *      Author: iizuka
 */

#include "bc_finalizer_trx/RegisterTicketTransaction.h"
#include "bc_finalizer_trx/TicketUtxo.h"

#include "bc_network/NodeIdentifier.h"

#include "base/StackRelease.h"

#include "crypto/Sha256.h"

#include "bc_trx/TransactionId.h"

#include "bc_base/BinaryUtils.h"

#include "bc_status_cache_context/IStatusCacheContext.h"

namespace codablecash {

RegisterTicketTransaction::RegisterTicketTransaction(const RegisterTicketTransaction &inst)
		: AbstractFinalizerTransaction(inst) {
	this->ticketUtxo = dynamic_cast<TicketUtxo*>(inst.ticketUtxo->copyData());
}

RegisterTicketTransaction::RegisterTicketTransaction() : AbstractFinalizerTransaction() {
	this->ticketUtxo = new TicketUtxo();
}

RegisterTicketTransaction::~RegisterTicketTransaction() {
	delete this->ticketUtxo;
}

bool RegisterTicketTransaction::validateOnAccept(MemPoolTransaction *memTrx, IStatusCacheContext *context) const {
	UtxoValidationResult utxoRes = validateUtxos(memTrx, context, this->fee);
	if(utxoRes == UtxoValidationResult::INVALID){
		return false;
	}

	{
		const NodeIdentifier* nodeId = this->ticketUtxo->getNodeIdentifier();
		const VoterEntry* entry = context->getVoterEntry(nodeId);
		if(entry == nullptr){
			return false;
		}
	}

	return verify();
}

TrxValidationResult RegisterTicketTransaction::validateFinal(
		const BlockHeader *header, MemPoolTransaction *memTrx, IStatusCacheContext *context) const {
	{
		uint64_t priceUint = context->getTicketPrice();
		BalanceUnit price(priceUint);
		BalanceUnit amount = this->ticketUtxo->getAmount();
		if(price.compareTo(&amount) > 0){
			return TrxValidationResult::INVALID;
		}
	}

	{
		const NodeIdentifier* nodeId = this->ticketUtxo->getNodeIdentifier();
		const VoterEntry* entry = context->getVoterEntry(nodeId);
		if(entry == nullptr){
			return TrxValidationResult::INVALID;
		}
	}

	UtxoValidationResult utxoRes = validateUtxos(memTrx, context, this->fee);
	if(utxoRes == UtxoValidationResult::INVALID){
		return TrxValidationResult::INVALID;
	}
	else if(utxoRes == UtxoValidationResult::ON_MEMORY){
		return TrxValidationResult::PENDING;
	}

	return TrxValidationResult::OK;
}

void RegisterTicketTransaction::setNodeId(const NodeIdentifier *nodeId) noexcept {
	this->ticketUtxo->setNodeIndentifier(nodeId);
}

void RegisterTicketTransaction::setAddressDescriptor(const AddressDescriptor *addressDesc) noexcept {
	this->ticketUtxo->setAddressDescriptor(addressDesc);
}

void RegisterTicketTransaction::setAmounst(BalanceUnit amount) noexcept {
	this->ticketUtxo->setAmounst(amount);
}

uint8_t RegisterTicketTransaction::getType() const noexcept {
	return AbstractBlockchainTransaction::TRX_TYPE_REGISTER_TICKET;
}

int RegisterTicketTransaction::binarySize() const {
	int total = AbstractControlTransaction::binarySize();

	total += this->ticketUtxo->binarySize();

	return total;
}

void RegisterTicketTransaction::toBinary(ByteBuffer *out) const {
	AbstractControlTransaction::toBinary(out);

	this->ticketUtxo->toBinary(out);
}

void RegisterTicketTransaction::fromBinary(ByteBuffer *in) {
	AbstractControlTransaction::fromBinary(in);

	AbstractUtxo* utxo = AbstractUtxo::createFromBinary(in); __STP(utxo);
	BinaryUtils::checkNotNull(dynamic_cast<TicketUtxo*>(utxo));

	delete this->ticketUtxo;
	this->ticketUtxo = dynamic_cast<TicketUtxo*>(__STP_MV(utxo));
}

void RegisterTicketTransaction::build() {
	setUtxoNonce();

	int capacity = __binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(capacity, true); __STP(buff);

	__toBinary(buff);
	buff->position(0);

	ByteBuffer* sha = Sha256::sha256(buff, true); __STP(sha);

	delete this->trxId;
	this->trxId = new TransactionId((const char*)sha->array(), sha->limit());
}

IBlockObject* RegisterTicketTransaction::copyData() const noexcept {
	return new RegisterTicketTransaction(*this);
}

int RegisterTicketTransaction::getUtxoSize() const noexcept {
	return AbstractControlTransaction::getUtxoSize() + 1;
}

AbstractUtxo* RegisterTicketTransaction::getUtxo(int i) const noexcept {
	if(i == AbstractControlTransaction::getUtxoSize()){
		return this->ticketUtxo;
	}

	return AbstractControlTransaction::getUtxo(i);
}

} /* namespace codablecash */
