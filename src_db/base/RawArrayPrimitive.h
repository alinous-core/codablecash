/*
 * RawArrayPrimitive.h
 *
 *  Created on: 2018/04/13
 *      Author: iizuka
 */

#ifndef BASE_RAWARRAYPRIMITIVE_H_
#define BASE_RAWARRAYPRIMITIVE_H_

#include "osenv/memory.h"


namespace alinous {

template <typename T>
class RawArrayPrimitive {
public:
	int numArray;
	int currentSize;
	T* root;
private:
	bool sorted;


private:
	T* allocNewRoot(int allocsize){
		T* p = new T[allocsize];
		Mem::memset(p, 0, sizeof(T) * allocsize);

		return p;
	}
public:
	RawArrayPrimitive(const RawArrayPrimitive& inst) = delete;
	explicit RawArrayPrimitive(int defaultSize) noexcept : numArray(0),
			currentSize(defaultSize > 4 ? defaultSize : 4),
			root(allocNewRoot(currentSize)),
			//root(new T[currentSize]{}),
			sorted(false){
	}

	~RawArrayPrimitive() noexcept {
		delete [] root;
	}

	void backLast(){
		this->numArray--;
	}

	void reset() noexcept {
		this->numArray = 0;
		this->sorted = false;
	}

	void addElement(const T value) noexcept {
		if(__builtin_expect(!(this->currentSize > this->numArray), 0)){
			int size = this->currentSize * 2;

			T* newPtr =new T[size];
			//__memset(newPtr, 0, sizeof(T) * size);

			Mem::memcpy(newPtr, this->root, sizeof(T) * this->currentSize);

			delete [] this->root;
			this->root = newPtr;

			this->currentSize = size;
		}

		this->root[this->numArray++] = value;

		this->sorted = false;
	}

	void addElement(const T value, const int index) noexcept {
		if(__builtin_expect(!(this->currentSize > this->numArray), 0)){
			int size = this->currentSize * 2;

			T* newPtr = new T[size];

			Mem::memcpy(newPtr, this->root, sizeof(T) * this->currentSize);

			delete [] this->root;

			this->root = newPtr;
			this->currentSize = size;
		}

		int copySize = this->numArray - index;
		if(copySize > 0){
			for(int i = this->numArray; i > index; i--){
				this->root[i] = this->root[i - 1];
			}
		}

		this->numArray++;
		this->root[index] = value;
	}


	inline int size() const noexcept {
		return numArray;
	}
	inline T get(const int i) const noexcept {
		return *(this->root + i);
	}

	inline T* getRoot() const noexcept {
		return this->root;
	}

	inline void set(const int i, T value) const noexcept {
		*(this->root + i) = value;
	}

	inline void setNumArray(int numArray) noexcept {
		this->numArray = numArray;
	}

	void remove(const int index) noexcept {
		remove(index, 1);
	}

	void remove(const int index, const int length) noexcept {
		#ifdef __DEBUG__
		//assert((index + length) <= this->numArray);
		#endif

		const int copySize = (this->numArray - index - length);
		if(copySize > 0){
			for(int i = 0; i < copySize; i++){
				this->root[index + i] = this->root[index + i + length];
			}
		}

		this->numArray = this->numArray - length;
	}

	inline T pop() noexcept {
		int pos = this->numArray - 1;
		T ret = get(pos);

		remove(pos);

		return ret;
	}

};

}

#endif /* BASE_RAWARRAYPRIMITIVE_H_ */
