/*
 * RegisterVotePoolTransaction.cpp
 *
 *  Created on: 2023/04/15
 *      Author: iizuka
 */

#include "bc_finalizer_trx/RegisterVotePoolTransaction.h"

#include "bc_network/NodeIdentifier.h"
#include "bc_network/NodeIdentifierSource.h"

#include "bc_trx/TransactionId.h"

#include "base/StackRelease.h"

#include "crypto/Sha256.h"
#include "crypto/SchnorrSignature.h"

#include "bc_base/AddressDescriptor.h"
#include "bc_base/BinaryUtils.h"

#include "crypto/Schnorr.h"

#include "bc_finalizer/VoterEntry.h"

#include "bc_status_cache_context/IStatusCacheContext.h"

namespace codablecash {

RegisterVotePoolTransaction::RegisterVotePoolTransaction() : AbstractFinalizerTransaction() {
	this->nodeId = nullptr;
	this->addressDesc = nullptr;
	this->voterSig = nullptr;
}

RegisterVotePoolTransaction::RegisterVotePoolTransaction(const RegisterVotePoolTransaction &inst)
		: AbstractFinalizerTransaction(inst) {
	this->nodeId = dynamic_cast<NodeIdentifier*>(inst.nodeId->copyData());
	this->addressDesc = inst.addressDesc != nullptr ? new AddressDescriptor(*inst.addressDesc) : nullptr;
	this->voterSig = inst.voterSig != nullptr ? new SchnorrSignature(*inst.voterSig) : nullptr;
}

RegisterVotePoolTransaction::RegisterVotePoolTransaction(const NodeIdentifier *nodeId, const AddressDescriptor* addressDesc)
		: AbstractFinalizerTransaction(){
	this->nodeId = dynamic_cast<NodeIdentifier*>(nodeId->copyData());
	this->addressDesc = new AddressDescriptor(*addressDesc);
	this->voterSig = nullptr;
}

RegisterVotePoolTransaction::~RegisterVotePoolTransaction() {
	delete this->nodeId;
	delete this->addressDesc;
	delete this->voterSig;
}

uint8_t RegisterVotePoolTransaction::getType() const noexcept {
	return AbstractBlockchainTransaction::TRX_TYPE_REGISTER_VOTE_POOL;
}

int RegisterVotePoolTransaction::binarySize() const {
	BinaryUtils::checkNotNull(this->nodeId);
	BinaryUtils::checkNotNull(this->addressDesc);

	int total = AbstractControlTransaction::binarySize();

	total += this->nodeId->binarySize();
	total += this->addressDesc->binarySize();

	total += sizeof(uint8_t);
	if(this->voterSig != nullptr){
		total += this->voterSig->binarySize();
	}

	return total;
}

void RegisterVotePoolTransaction::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->nodeId);
	BinaryUtils::checkNotNull(this->addressDesc);

	AbstractControlTransaction::toBinary(out);

	this->nodeId->toBinary(out);
	this->addressDesc->toBinary(out);

	bool bl = this->voterSig != nullptr;
	out->put(bl ? 1 : 0);
	if(bl){
		this->voterSig->toBinary(out);
	}
}

void RegisterVotePoolTransaction::fromBinary(ByteBuffer *in) {
	AbstractControlTransaction::fromBinary(in);

	this->nodeId = NodeIdentifier::fromBinary(in);
	this->addressDesc = AddressDescriptor::createFromBinary(in);

	uint8_t bl = in->get();
	if(bl > 0){
		this->voterSig = SchnorrSignature::createFromBinary(in);
	}
}

void RegisterVotePoolTransaction::build() {
	setUtxoNonce();

	int capacity = __binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(capacity, true); __STP(buff);

	__toBinary(buff);
	buff->position(0);

	ByteBuffer* sha = Sha256::sha256(buff, true); __STP(sha);

	delete this->trxId;
	this->trxId = new TransactionId((const char*)sha->array(), sha->limit());
}

IBlockObject* RegisterVotePoolTransaction::copyData() const noexcept {
	return new RegisterVotePoolTransaction(*this);
}

bool RegisterVotePoolTransaction::validateOnAccept(MemPoolTransaction *memTrx, IStatusCacheContext *context) const {
	{
		const VoterEntry* entry = context->getVoterEntry(this->nodeId);
		if(entry != nullptr){
			return false;
		}
	}

	UtxoValidationResult utxoRes = validateUtxos(memTrx, context, this->fee);
	if(utxoRes == UtxoValidationResult::INVALID){
		return false;
	}

	return verify() && verifyVoterSign();
}

TrxValidationResult RegisterVotePoolTransaction::validateFinal(
		const BlockHeader *header, MemPoolTransaction *memTrx, IStatusCacheContext *context) const {
	{
		const VoterEntry* entry = context->getVoterEntry(this->nodeId);
		if(entry != nullptr){
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

void RegisterVotePoolTransaction::voterSign(const NodeIdentifierSource *source) noexcept {
	delete this->voterSig;

	const char* data = this->trxId->toArray();
	int length = this->trxId->size();

	this->voterSig = Schnorr::sign(*source->getSecretKey(), *this->nodeId->getPublicKey(), (const uint8_t*)data, (size_t)length);
}

bool RegisterVotePoolTransaction::verifyVoterSign() const noexcept {
	const char* data = this->trxId->toArray();
	int length = this->trxId->size();

	bool result = Schnorr::verify(*this->voterSig, *this->nodeId->getPublicKey(), (const uint8_t*)data, (size_t)length);
	return result;
}


} /* namespace codablecash */
