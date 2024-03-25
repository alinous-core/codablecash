/*
 * ConnectNodeSelector.cpp
 *
 *  Created on: 2023/10/19
 *      Author: iizuka
 */

#include "bc_p2p_selector/ConnectNodeSelector.h"

#include "bc_p2p_info/P2pNodeRecord.h"

#include "osenv/funcs.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"

#include "crypto/Sha256Context.h"

#include "numeric/BigInteger.h"
using namespace  alinous;

namespace codablecash {

ConnectNodeSelector::ConnectNodeSelector() {
	this->candidates = new ArrayList<P2pNodeRecord>();
	this->usedCandidates = new ArrayList<P2pNodeRecord>();
	this->randomValue = nullptr;
}

ConnectNodeSelector::~ConnectNodeSelector() {
	this->candidates->deleteElements();
	delete this->candidates;

	this->usedCandidates->deleteElements();
	delete this->usedCandidates;

	delete this->randomValue;
}

void ConnectNodeSelector::importCandidates(const ArrayList<P2pNodeRecord> *list) {
	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		P2pNodeRecord* r = list->get(i);

		this->candidates->addElement(dynamic_cast<P2pNodeRecord*>(r->copyData()));
	}

	uint64_t tm = Os::getTimestampLong();

	int capacity = sizeof(tm);
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(capacity, true); __STP(buff);
	buff->putLong(tm);
	buff->position(0);

	Sha256Context sha;
	sha.update((const char*)buff->array(), buff->limit());
	sha.finalize();

	const uint8_t* data = sha.getData32();

	this->randomValue = BigInteger::fromBinary((const char*)data, 32);
}

bool ConnectNodeSelector::hasNext() const noexcept {
	return !this->candidates->isEmpty();
}

const P2pNodeRecord* ConnectNodeSelector::fetchNext() const {
	int index = toIndex(this->candidates->size());

	P2pNodeRecord* record = this->candidates->remove(index);
	this->usedCandidates->addElement(record);

	return record;
}

int ConnectNodeSelector::toIndex(int listsize) const noexcept {
	BigInteger mod(listsize);

	BigInteger res = this->randomValue->mod(mod);

	return res.longValue();
}

} /* namespace codablecash */
