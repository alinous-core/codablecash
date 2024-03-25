/*
 * BlockHeaderList.cpp
 *
 *  Created on: 2023/02/16
 *      Author: iizuka
 */

#include "bc_blockstore_header/BlockHeaderList.h"

namespace codablecash {

BlockHeaderList::BlockHeaderList(const BlockHeaderList &inst): list(4) {
	int maxLoop = inst.list.size();
	for(int i = 0; i != maxLoop; ++i){
		uint64_t fpos = inst.list.get(i);

		this->list.addElement(fpos);
	}
}

BlockHeaderList::BlockHeaderList() : list(4) {
}

BlockHeaderList::~BlockHeaderList() {

}

int BlockHeaderList::binarySize() const {
	int total = sizeof(uint32_t);
	total += sizeof(uint64_t) * this->list.size();

	return total;
}

void BlockHeaderList::toBinary(ByteBuffer *out) const {
	int maxLoop = this->list.size();
	out->putInt(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		uint64_t fpos = this->list.get(i);
		out->putLong(fpos);
	}
}

BlockHeaderList* BlockHeaderList::fromBinary(ByteBuffer *in) {
	BlockHeaderList* value = new BlockHeaderList();

	int maxLoop = in->getInt();
	for(int i = 0; i != maxLoop; ++i){
		uint64_t fpos = in->getLong();
		value->list.addElement(fpos);
	}

	return value;
}

IBlockObject* BlockHeaderList::copyData() const noexcept {
	return new BlockHeaderList(*this);
}

void BlockHeaderList::join(const BlockHeaderList *value) noexcept {
	const RawArrayPrimitive<uint64_t>* newList = &value->list;

	int maxLoop = newList->size();
	for(int i = 0; i != maxLoop; ++i){
		uint64_t v = newList->get(i);
		if(contains(v)){
			continue;
		}
		this->list.addElement(v);
	}
}

bool BlockHeaderList::contains(uint64_t value) const noexcept {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		uint64_t v = this->list.get(i);
		if(v == value){
			return true;
		}
	}

	return false;
}

void BlockHeaderList::remove(uint64_t value) noexcept {
	int index = indexof(value);

	if(index >= 0){
		this->list.remove(index);
	}
}

int BlockHeaderList::indexof(uint64_t value) const noexcept {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		uint64_t v = this->list.get(i);
		if(v == value){
			return i;
		}
	}

	return -1;
}

bool BlockHeaderList::isEmpty() const noexcept {
	return this->list.size() == 0;
}

void BlockHeaderList::add(uint64_t value) noexcept {
	this->list.addElement(value);
}

int BlockHeaderList::size() const noexcept {
	return this->list.size();
}

uint64_t BlockHeaderList::get(int pos) const noexcept {
	return this->list.get(pos);
}

} /* namespace codablecash */
