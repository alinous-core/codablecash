/*
 * VoterEntry.cpp
 *
 *  Created on: 2023/04/15
 *      Author: iizuka
 */

#include "bc_finalizer/VoterEntry.h"
#include "bc_finalizer/VoteTicket.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_base/AddressDescriptor.h"
#include "bc_base/BinaryUtils.h"

#include "bc_trx/UtxoId.h"

#include "base/StackRelease.h"

#include "merkletree/MerkleCertificate.h"


namespace codablecash {

VoterEntry::VoterEntry(const VoterEntry &inst) {
	this->nodeId = dynamic_cast<NodeIdentifier*>(inst.nodeId->copyData());
	this->missingCount = inst.missingCount;
	this->extendCount = inst.extendCount;
	this->capacity = inst.capacity;

	this->list = new ArrayList<VoteTicket>();
	{
		int maxLoop = inst.list->size();
		for(int i = 0; i != maxLoop; ++i){
			VoteTicket* ticket = inst.list->get(i);

			this->list->addElement(new VoteTicket(*ticket));
		}
	}

	this->pos = inst.pos;

	this->voterAddressDesc = inst.voterAddressDesc != nullptr ? new AddressDescriptor(*inst.voterAddressDesc) : nullptr;
	this->updated = inst.updated;

	this->nodeIdMerkleCert = inst.nodeIdMerkleCert != nullptr ? dynamic_cast<MerkleCertificate*>(inst.nodeIdMerkleCert->copyData()) : nullptr;
}

VoterEntry::VoterEntry() {
	this->nodeId = nullptr;
	this->missingCount = 0;
	this->extendCount = 0;
	this->capacity = 0;

	this->list = new ArrayList<VoteTicket>();
	this->pos = 0;

	this->voterAddressDesc = nullptr;
	this->updated = false;

	this->nodeIdMerkleCert = nullptr;
}

VoterEntry::~VoterEntry() {
	delete this->nodeId;

	this->list->deleteElements();
	delete this->list;

	delete this->voterAddressDesc;

	delete this->nodeIdMerkleCert;
}

VoterEntry* VoterEntry::createVoteEntry(const NodeIdentifier *nodeId, const AddressDescriptor* voterAddress) {
	VoterEntry* entry = new VoterEntry();
	entry->setNodeIdentifier(nodeId);
	entry->setAddressDesc(voterAddress);

	return entry;
}

void VoterEntry::setNodeIdentifier(const NodeIdentifier *nodeId) noexcept {
	delete this->nodeId;
	this->nodeId = dynamic_cast<NodeIdentifier*>(nodeId->copyData());
	this->updated = true;
}

void VoterEntry::setCapacity(int cap) {
	this->capacity = cap;
	this->updated = true;
}

void VoterEntry::setAddressDesc(const AddressDescriptor *addressDesc) noexcept {
	delete this->voterAddressDesc;
	this->voterAddressDesc = new AddressDescriptor(*addressDesc);
	this->updated = true;
}

void VoterEntry::addTicket(const VoteTicket *ticket) noexcept {
	this->list->addElement(new VoteTicket(*ticket));
	this->updated = true;
}

bool VoterEntry::removeTicket(const UtxoId *utxoId) noexcept {
	int index = -1;

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		VoteTicket* ticket = this->list->get(i);
		const UtxoId* id = ticket->getUtxoId();

		if(id->equals(utxoId)){
			index = i;
			break;
		}
	}

	if(index >= 0){
		delete this->list->remove(index);
	}

	this->updated = true;
	return index != -1;
}

const VoteTicket* VoterEntry::getTicket(const UtxoId *utxoId) const {
	const VoteTicket* ret = nullptr;

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		VoteTicket* ticket = this->list->get(i);

		const UtxoId* id = ticket->getUtxoId();
		if(id->equals(utxoId)){
			ret = ticket;
			break;
		}
	}

	return ret;
}

int VoterEntry::VoteCompare::operator ()(const VoterEntry *const a, const VoterEntry *const b) const noexcept {
	return a->nodeId->compareTo(b->nodeId);
}

const VoteTicket* VoterEntry::nextTicket(uint64_t matureHeight) noexcept {
	int size = this->list->size();
	size = size >= this->capacity ? this->capacity : size;

	if(this->pos >= size || this->list->get(this->pos)->getHeight() > matureHeight){
		return nullptr;
	}

	return this->list->get(this->pos++);
}

void VoterEntry::handleVoted(int maxextendCount) noexcept {
	this->missingCount = 0;

	this->extendCount++;
	if(this->extendCount >= maxextendCount){
		this->capacity++;
		this->extendCount = 0;
	}

	this->updated = true;
}

void VoterEntry::handleMissed(int missingLimit) noexcept {
	this->extendCount = 0;

	this->missingCount++;
	if(this->missingCount >= missingLimit){
		this->capacity = this->capacity / 2;
		this->missingCount = 0;
	}

	this->updated = true;
}

#ifdef __DEBUG__

BalanceUnit VoterEntry::getTicketPriceSum() const noexcept {
	BalanceUnit total(0L);

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		VoteTicket* ticket = this->list->get(i);

		total += ticket->getTicketPrice();
	}

	return total;
}

int VoterEntry::countTicket(const UtxoId* utxoid) const {
	int count = 0;

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		VoteTicket* ticket = this->list->get(i);

		const UtxoId* id = ticket->getUtxoId();
		if(utxoid->equals(id)){
			count++;
		}
	}

	return count;
}

bool VoterEntry::checkTickets() const {
	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		VoteTicket* ticket = this->list->get(i);

		const UtxoId* id = ticket->getUtxoId();
		int count = countTicket(id);

		assert(count == 1);
		//if(count != 1){
		//	return false;
		//}
	}

	return true;
}
#endif

int VoterEntry::binarySize() const {
	BinaryUtils::checkNotNull(this->nodeId);
	BinaryUtils::checkNotNull(this->voterAddressDesc);

	int total = this->nodeId->binarySize();
	total += this->voterAddressDesc->binarySize();
	total += sizeof(this->missingCount) + sizeof(this->extendCount) + sizeof(this->capacity);

	int maxLoop = this->list->size();
	total += sizeof(uint32_t);

	for(int i = 0; i != maxLoop; ++i){
		VoteTicket* ticket = this->list->get(i);
		total += ticket->binarySize();
	}


	total += sizeof(uint8_t);
	if(this->nodeIdMerkleCert != nullptr){
		total += this->nodeIdMerkleCert->binarySize();
	}

	return total;
}

void VoterEntry::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->nodeId);
	BinaryUtils::checkNotNull(this->voterAddressDesc);

	this->nodeId->toBinary(out);
	this->voterAddressDesc->toBinary(out);

	out->putInt(this->missingCount);
	out->putInt(this->extendCount);
	out->putInt(this->capacity);

	int maxLoop = this->list->size();
	out->putInt(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		VoteTicket* ticket = this->list->get(i);
		ticket->toBinary(out);
	}

	uint8_t bl = this->nodeIdMerkleCert != nullptr ? 1 : 0;
	out->put(bl);
	if(bl > 0){
		this->nodeIdMerkleCert->toBinary(out);
	}
}

VoterEntry* VoterEntry::createFromBinary(ByteBuffer *in) {
	VoterEntry* entry = new VoterEntry(); __STP(entry);

	entry->nodeId = NodeIdentifier::fromBinary(in);
	entry->voterAddressDesc = AddressDescriptor::createFromBinary(in);

	entry->missingCount = in->getInt();
	entry->extendCount = in->getInt();
	entry->capacity = in->getInt();

	int maxLoop = in->getInt();
	for(int i = 0; i != maxLoop; ++i){
		VoteTicket* ticket = VoteTicket::createFromBinary(in); __STP(ticket);
		entry->addTicket(ticket);
	}

	uint8_t bl = in->get();
	if(bl > 0){
		entry->nodeIdMerkleCert = MerkleCertificate::createFromBinary(in);
	}

	// clear update flag
	entry->setUpdated(false);

	return __STP_MV(entry);
}

IBlockObject* VoterEntry::copyData() const noexcept {
	return new VoterEntry(*this);
}

void VoterEntry::setNodeIdMerkleCert(const MerkleCertificate *cert) noexcept {
	delete this->nodeIdMerkleCert;
	this->nodeIdMerkleCert = dynamic_cast<MerkleCertificate*>(cert->copyData());

	this->updated = true;
}

} /* namespace codablecash */
