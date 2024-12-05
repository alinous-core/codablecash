/*
 * BlockHead.cpp
 *
 *  Created on: 2023/03/19
 *      Author: iizuka
 */

#include "bc_status_cache/BlockHead.h"
#include "bc_status_cache/BlockHeadElement.h"

#include "bc_block/BlockHeader.h"

#include "numeric/BigInteger.h"

#include "pow/PoWNonce.h"

#include "bc/ISystemLogger.h"

#include "bc_status_cache/BlockHeadElementPadding.h"

#include "base/ArrayList.h"
#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "base_timestamp/SystemTimestamp.h"
using namespace alinous;

namespace codablecash {

/*
BlockHead::BlockHead(const BlockHead &inst) {
	int maxLoop = inst.list.size();
	for(int i = 0; i != maxLoop; ++i){
		const BlockHeadElement* h = inst.list.get(i);
		add(h->getBlockHeader());
	}
}*/

BlockHead::BlockHead() {

}

BlockHead::~BlockHead() {
	this->list.deleteElements();
}

BlockHead* BlockHead::copyOriginalBlockHead() const noexcept {
	BlockHead* newHead = new BlockHead(); __STP(newHead);

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		const BlockHeadElement* h = this->list.get(i);

		if(h->isPaddong()){
			break;
		}

		newHead->add(h);
	}

	return __STP_MV(newHead);
}

void BlockHead::normalizeWithlength(int length) {
	int maxLoop = this->list.size();

	int paddingSize = length - maxLoop;
	for(int i = 0; i != paddingSize; ++i){
		BlockHeadElementPadding* pad = new BlockHeadElementPadding();

		this->list.addElement(pad);
	}
}

void BlockHead::add(const BlockHeader *header) noexcept {
	BlockHeadElement* element = new BlockHeadElement(header);

	this->list.addElement(element);
}

void BlockHead::add(const BlockHeadElement* newElement) noexcept {
	const BlockHeadElement& inst = *newElement;
	BlockHeadElement* element = new BlockHeadElement(inst);

	this->list.addElement(element);
}

const BlockHeader* BlockHead::getHeadHeader() const noexcept {
	BlockHeadElement* head = getTopBlockHeadElement(0);
	assert(head != nullptr);

	return head->getBlockHeader();
}

const BlockHeader* BlockHead::getRealHeadHeader() const noexcept {
	const BlockHeader* header = getHeadHeader();

	if(header->isScheduledBlock()){
		BlockHeadElement* head = getTopBlockHeadElement(1);
		header = head->getBlockHeader();
	}
	assert(header != nullptr);

	return header;
}

uint64_t BlockHead::getHeadHeight() const noexcept {
	const BlockHeader* h = getHeadHeader();
	return h->getHeight();
}

int BlockHeadCompare::operator ()(const BlockHead *const a,	const BlockHead *const b) const noexcept {
	return a->compareTo(b);
}

int BlockHead::compareTo(const BlockHead *other) const noexcept {
	int otherMaxLoop = other->size();
	int maxLoop = size();

	assert(otherMaxLoop == maxLoop);

	// compare by Vote -> VTP -> Mev -> length -> timestamp
	int diff = 0;

	// check by voted score
	diff = compareToByVoted(other);
	if(diff != 0){
		return diff;
	}

	// check by voting
	diff = compareToByVoting(other);
	if(diff != 0){
		return diff;
	}

	// check by total mev score
	diff = compareToByTotalMevScore(other);
	if(diff != 0){
		return diff;
	}

	// check by length
	diff = compareToByLastHeight(other);
	if(diff != 0){
		return diff;
	}

	// check by last timestamp
	return compareToByLastTimestamp(other);
}

int BlockHead::compareToByVoted(const BlockHead *other) const noexcept {
	int diff = 0;

	int maxLoop = size();
	for(int i = 0; i != maxLoop; ++i){
		BlockHeadElement* block = this->list.get(i);
		BlockHeadElement* otherBlock = other->list.get(i);

		int v = block->getVotedScore();
		int vo = otherBlock->getVotingSocre();

		diff = v - vo;
		if(diff != 0){
			break;
		}
	}

	return diff;
}

int BlockHead::compareToByVoting(const BlockHead *other) const noexcept {
	int diff = 0;

	int maxLoop = size();
	for(int i = 0; i != maxLoop; ++i){
		BlockHeadElement* block = this->list.get(i);
		BlockHeadElement* otherBlock = other->list.get(i);

		int v = block->getVotingSocre();
		int vo = otherBlock->getVotingSocre();

		diff = v - vo;
		if(diff != 0){
			break;
		}
	}

	return diff;
}

int BlockHead::compareToByTotalMevScore(const BlockHead *other) const noexcept {
	int64_t v  = 0;
	int64_t vo  = 0;

	int maxLoop = size();
	for(int i = 0; i != maxLoop; ++i){
		BlockHeadElement* block = this->list.get(i);
		BlockHeadElement* otherBlock = other->list.get(i);

		v += block->getMevHealthScore();
		vo += otherBlock->getMevHealthScore();
	}

	return v - vo;
}

int BlockHead::compareToByLastHeight(const BlockHead *other) const noexcept {
	const BlockHeader* header = getHeadHeader();
	uint64_t height = header->getHeight();

	const BlockHeader* otherHeader = other->getHeadHeader();
	uint64_t otherHeight = otherHeader->getHeight();

	return height > otherHeight ? 1 : (height < otherHeight ? -1 : 0);
}

int BlockHead::compareToByLastTimestamp(const BlockHead *other) const noexcept {
	const BlockHeader* header = getHeadHeader();
	const SystemTimestamp* tm = header->getTimestamp();

	const BlockHeader* otherHeader = other->getHeadHeader();
	const SystemTimestamp* othertm = otherHeader->getTimestamp();

	return tm->compareTo(othertm);
}

BlockHeadElement* BlockHead::getTopBlockHeadElement(int posFromTop) const noexcept {
	int count = 0;

	int maxLoop = size();
	for(int i = 0; i != maxLoop; ++i){
		BlockHeadElement* block = this->list.get(i);

		if(block->isPaddong()){
			break;
		}
		count = i;
	}

	return this->list.get(count - posFromTop);
}


void BlockHead::logStatus(ISystemLogger *logger) const {
	UnicodeString message(L"---- Block Head -----\n");

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		BlockHeadElement* element = this->list.get(i);
		if(element->isPaddong()){
			break;
		}

		UnicodeString* str = element->toString(); __STP(str);
		message.append(L" ").append(str).append(L"\n");
	}

	message.append(L"----------------------");


	logger->debugLog(ISystemLogger::DEBUG_CHAIN_HEAD_DETECT, &message, __FILE__, __LINE__);
}

const BlockHeadElement* BlockHead::getHeadBlockHead() const noexcept {
	int pos = this->list.size() - 1;
	return this->list.get(pos);
}

} /* namespace codablecash */
