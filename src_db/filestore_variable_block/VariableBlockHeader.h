/*
 * VariableBlockHeader.h
 *
 *  Created on: 2023/02/08
 *      Author: iizuka
 */

#ifndef FILESTORE_VARIABLE_BLOCK_VARIABLEBLOCKHEADER_H_
#define FILESTORE_VARIABLE_BLOCK_VARIABLEBLOCKHEADER_H_

#include <cstdint>

namespace alinous {

class RandomAccessFile;
class LongRangeList;
class VariableBlock;
class LongRange;

class VariableBlockHeader {
public:
	VariableBlockHeader(const VariableBlockHeader& base) = delete;
	explicit VariableBlockHeader(RandomAccessFile* file);
	virtual ~VariableBlockHeader();

	void createStore(bool del, uint64_t defaultSize, uint64_t blockUnitSize, uint64_t extendBlocks);

	void sync(bool fileSync=true);
	void sync2File();
	void loadFromFile();

	uint64_t getBlockUnitSize() const noexcept {
		return this->blockUnitSize;
	}

	VariableBlock* reallocFirstMaxFragment(uint64_t firstBlockPos, uint64_t size);
	VariableBlock* allocMaxFragment(uint64_t size);
	VariableBlock* allocMaxFragment(const LongRange* range, uint64_t size);
	void freeFragment(const LongRange* range);
	bool isEmpty() const noexcept;

	uint64_t availableCapacity() const noexcept;

	uint64_t extend();

private:
	int getNumAllocationBlocks(uint64_t size);
	VariableBlock* allocateAll(const LongRange* range);
	uint64_t availableWithRange(const LongRange* range) const noexcept;

private:
	RandomAccessFile* file;
	LongRangeList* availableArea;
	uint64_t blockUnitSize;
	uint64_t numBlocks;
	uint64_t extendBlocks;
};

} /* namespace alinous */

#endif /* FILESTORE_VARIABLE_BLOCK_VARIABLEBLOCKHEADER_H_ */
