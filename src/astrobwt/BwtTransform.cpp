/*
 * BwtTransform.cpp
 *
 *  Created on: 2022/04/11
 *      Author: iizuka
 */

#include "BwtTransform.h"

#include "osenv/memory.h"

#include "base/ArrayList.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"
namespace codablecash {

BwtTransform::BwtTransform(int length) {
	this->sizeOfElement = sizeof(BwtElement);

	this->length = length;
	this->ptrPool = new char[sizeOfElement * this->length];

	this->list = new BwtElementArrayArray(0xff + 1);
}

BwtTransform::~BwtTransform() {
	delete this->list;
	delete [] this->ptrPool;
}

typedef ArrayList<BwtElement, BwtElement::ValueCompare> BTW_HASH_ARRAY;

void BwtTransform::transform(ByteBuffer* result, const uint8_t* data, int len) {
	this->list->reset();

	uint8_t* data2 = new uint8_t[len * 2]; StackArrayRelease<uint8_t> __st_data2(data2);
	Mem::memcpy(data2, data, len);
	Mem::memcpy(data2 + len, data, len);

	for(int i = 0; i != len; ++i){
		BwtElement* element = (BwtElement*)(ptrPool + (i * this->sizeOfElement));
		element->init(data2, len, i);

		this->list->addElement(element);
	}


	result->limit(len);
	result->position(0);
	this->list->putSuffix(result);
}

BwtElement::BwtElement(const uint8_t* newdata, int len, int start) {
	this->data = newdata;
	this->len = len;
	this->start = start;
}

static int check(int i, int maxLoop, const BwtElement* const a, const BwtElement* const b) noexcept;
int BwtElement::ValueCompare::operator ()(const BwtElement* const a, const BwtElement* const b) const noexcept {
	int ret_diff = 0;

	int maxLoop = a->len;
	int i = 0;

	const uint8_t* astart = a->data + a->start;
	const uint8_t* bstart = b->data + b->start;

	while((maxLoop - i) >= 8){
		//uint64_t ach = *((uint64_t*)(astart + i)); // endian
		//uint64_t bch = *((uint64_t*)(bstart + i));
		i += 8;

		const uint8_t* astart_i = astart + i;
		const uint8_t* bstart_i = bstart + i;
		uint64_t ach = ((uint64_t)*(astart + i) << 56) | ((uint64_t)*(astart + i + 1) << 48)
								| ((uint64_t)*(astart_i + 2) << 40) | ((uint64_t)*(astart_i + 3) << 32)
								| ((uint64_t)*(astart_i + 4) << 24) | ((uint64_t)*(astart_i + 5) << 16)
								| ((uint64_t)*(astart_i + 6) << 8)  | ((uint64_t)*(astart_i + 7));
		uint64_t bch = ((uint64_t)*(bstart_i) << 56) | ((uint64_t)*(bstart_i + 1) << 48)
								| ((uint64_t)*(bstart_i + 2) << 40) | ((uint64_t)*(bstart_i + 3) << 32)
								| ((uint64_t)*(bstart_i + 4) << 24) | ((uint64_t)*(bstart_i + 5) << 16)
								| ((uint64_t)*(bstart_i + 6) << 8)  | ((uint64_t)*(bstart_i + 7));

		if(ach != bch){
			ret_diff = ach - bch > 0 ? 1 : -1;
			break;
		}
	}

	return ret_diff != 0 ? ret_diff : check(i, maxLoop, a, b);
}

static int check(int i, int maxLoop, const BwtElement* const a, const BwtElement* const b) noexcept {
	uint8_t ach = 0;
	uint8_t bch = 0;
	int diff = 0;
	for(; i != maxLoop; ++i){
		ach = a->getDataAt(i);
		bch = b->getDataAt(i);

		diff = ach - bch;
		if(diff != 0){
			break;
		}
	}

	return diff;
}

BwtElementArrayArray::BwtElementArrayArray(int len) noexcept {
	this->list = new BwtElementArrayPtr[len];
	this->length = len;

	for(int i = 0; i != this->length; ++i){
		BwtElementArray* ptr = new BwtElementArray();
		this->list[i] = ptr;
	}
}

BwtElementArrayArray::~BwtElementArrayArray() {
	for(int i = 0; i != this->length; ++i){
		delete this->list[i];
	}
	delete [] this->list;
}

void BwtElementArrayArray::reset() noexcept {
	for(int i = 0; i != this->length; ++i){
		this->list[i]->reset();
	}
}

} /* namespace codablecash */
