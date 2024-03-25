/*
 * BwtTransform.h
 *
 *  Created on: 2022/04/11
 *      Author: iizuka
 */

#ifndef ASTROBWT_BWTTRANSFORM_H_
#define ASTROBWT_BWTTRANSFORM_H_

#include <cstdint>

#include "base/ArrayList.h"

#include "base_io/ByteBuffer.h"

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class BwtElementArrayArray;

class BwtTransform {
public:
	explicit BwtTransform(int length);
	~BwtTransform();

	void transform(ByteBuffer* result, const uint8_t* data, int len);

private:
	char* ptrPool;
	int length;
	int sizeOfElement;

	// use 0x00 - 0xff hash to skip sorting
	BwtElementArrayArray* list;
};

class BwtElement {
public:
	//BwtElement();
	BwtElement(const uint8_t* dataptr, int len, int start);

	inline void init(const uint8_t* dataptr, int len, int start) {
		this->data = dataptr;
		this->len = len;
		this->start = start;
	}

	inline uint8_t getDataAt(int pos) const noexcept {
		return this->data[this->start + pos];
	}
	inline uint16_t getFirstUShort() const noexcept {
		uint16_t* sptr = (uint16_t*)(this->data + this->start);
		return *sptr;
	}

	inline uint8_t getSuffix() const noexcept {
		return getDataAt(this->len - 1);
	}

private:
	const uint8_t* data;
	int len;
	int start;

public:
	class ValueCompare {
	public:
		int operator () (const BwtElement* const a, const BwtElement* const b) const noexcept;
	};
};


class BwtElementArray {
public:
	BwtElementArray() noexcept :list(256) {}
	~BwtElementArray(){
	}

	inline void addElement(BwtElement* ptr) noexcept {
		this->list.addElement(ptr);
	}

	void putSuffix(ByteBuffer* result){
		this->list.sort();

		int maxLoop = this->list.size();
		for(int i = 0; i != maxLoop; ++i){
			BwtElement* element = list.get(i);
			uint8_t ch = element->getSuffix();

			result->put(ch);
		}
	}

	bool isEmpty() const noexcept{
		return this->list.isEmpty();
	}

	void reset() noexcept {
		this->list.reset();
	}

private:
	ArrayList<BwtElement, BwtElement::ValueCompare> list;
};

typedef BwtElementArray* BwtElementArrayPtr;

class BwtElementArrayArray {
public:
	explicit BwtElementArrayArray(int len) noexcept;
	~BwtElementArrayArray();

	void addElement(BwtElement* element){
		uint16_t hashPos = element->getDataAt(0); // element->getFirstUShort();
		//hashPos = hashPos << 8;
		//uint16_t two = element->getDataAt(1);
		//hashPos = hashPos | two;

		BwtElementArray* ptr = this->list[hashPos];
		ptr->addElement(element);
	}

	void putSuffix(ByteBuffer* result) {
		for(int i = 0; i != this->length; ++i){
			BwtElementArray* ptr = this->list[i];

			if(!ptr->isEmpty()){
				ptr->putSuffix(result);
			}
		}
	}

	void reset() noexcept;

private:
	int length;
	BwtElementArray** list;
};

} /* namespace codablecash */

#endif /* ASTROBWT_BWTTRANSFORM_H_ */
