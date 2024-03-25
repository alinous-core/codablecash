/*
 * TicketVoteSelector.cpp
 *
 *  Created on: 2023/04/28
 *      Author: iizuka
 */

#include "bc_finalizer/TicketVoteSelector.h"

#include "bc/CodablecashConfig.h"

#include "bc_finalizer/VoteTicket.h"

#include "crypto/Sha256.h"

#include "base/StackRelease.h"

#include "base_io/ByteBuffer.h"

namespace codablecash {

TicketVoteSelector::TicketVoteSelector(ArrayList<VoterEntry, VoterEntry::VoteCompare> *list, uint64_t height, const CodablecashConfig* config) {
	this->list = list;
	this->height = height;

	this->candidateList = new ArrayList<const VoteTicket>();
	this->expiredList = new ArrayList<const VoteTicket>();
	this->selected = new ArrayList<const VoteTicket>();

	this->tiketMatureIntervalHeight = config->getTicketMatureIntervalHeight(height);
	this->ticketExpireHeight = config->getTicketExpireHeight(height);

	this->votePerBlock = config->getVotePerBlock();
}

TicketVoteSelector::~TicketVoteSelector() {
	this->list = nullptr;

	delete this->candidateList;
	delete this->expiredList;
	delete this->selected;
}

void TicketVoteSelector::select() {
	this->list->sort();

	makeList();
	doSelect();
}


void TicketVoteSelector::doSelect() {
	int expCount = this->expiredList->size();
	expCount = expCount >= this->votePerBlock ? this->votePerBlock : expCount;

	int candidateCount = this->votePerBlock - expCount;
	candidateCount = candidateCount > this->candidateList->size() ? this->candidateList->size() : candidateCount;

	ByteBuffer* shabuff = makebuffer(); __STP(shabuff);

	int count = 0;
	for(int i = 0; i != expCount; ++i){
		const VoteTicket* ticket = selectFromList(this->expiredList, count, shabuff);
		this->selected->addElement(ticket);
		count++;
	}

	for(int i = 0; i != candidateCount; ++i){
		const VoteTicket* ticket = selectFromList(this->candidateList, count, shabuff);
		this->selected->addElement(ticket);
		count++;
	}

}

const VoteTicket* TicketVoteSelector::selectFromList(ArrayList<const VoteTicket> *list, int count, ByteBuffer* shabuff) {
	uint16_t pos = count % 32;
	pos = shabuff->getShort(pos);

	pos  = count % 32;
	pos = shabuff->getShort(pos);

	pos = pos % list->size();

	return list->remove(pos);
}

ByteBuffer* TicketVoteSelector::makebuffer() {
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(sizeof(uint64_t), true); __STP(buff);
	buff->putLong(this->height);
	buff->position(0);

	// Sha
	ByteBuffer* shabuff = Sha256::sha256(buff, true); __STP(shabuff);
	shabuff->put((char)0);
	shabuff->position(0);

	return __STP_MV(shabuff);
}

void TicketVoteSelector::makeList() {
	resetPosition();

	uint64_t matureHeight = this->height - this->tiketMatureIntervalHeight;

	bool added = false;
	do{
		added = false;

		int maxLoop = this->list->size();
		for(int i = 0; i != maxLoop; ++i){
			VoterEntry* entry = this->list->get(i);

			const VoteTicket* ticket = entry->nextTicket(matureHeight);
			if(ticket != nullptr){
				addList(ticket);
				added = true;
			}
		}
	}
	while(added);
}

void TicketVoteSelector::addList(const VoteTicket *ticket) {
	uint64_t ticketHeight = ticket->getHeight();
	uint64_t expire = this->height - ticketHeight;

	if(expire < this->ticketExpireHeight){
		this->candidateList->addElement(ticket);
	}
	else{
		this->expiredList->addElement(ticket);
	}
}

void TicketVoteSelector::resetPosition() {
	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		VoterEntry* entry = this->list->get(i);
		entry->setPosition(0);
	}
}
} /* namespace codablecash */
