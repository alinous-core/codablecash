/*
 * VariableBlock.h
 *
 *  Created on: 2023/02/08
 *      Author: iizuka
 */

#ifndef FILESTORE_VARIABLE_BLOCK_VARIABLEBLOCK_H_
#define FILESTORE_VARIABLE_BLOCK_VARIABLEBLOCK_H_

#include <cstdint>

namespace alinous {

class VariableBlockFileBody;
class VariableBlockHeader;
class LongRange;

class VariableBlock {
public:
	static constexpr int HEADER_SIZE{sizeof(uint16_t) + sizeof(uint64_t)};

	VariableBlock(uint16_t blockSize, uint64_t fpos, uint16_t used, uint64_t nextfpos, const char* data);
	virtual ~VariableBlock();

	static VariableBlock* load(VariableBlockFileBody *body, uint64_t fpos, uint16_t blockunit);

	void write(const char* buff, int length);
	void writeBack(VariableBlockFileBody* body);

	void freeBlock(VariableBlockHeader* header, VariableBlockFileBody* body);
	LongRange* getLongRange(uint64_t blockUnitSize) const noexcept;

	inline uint16_t headerSize() noexcept {
		return HEADER_SIZE;
	}
	inline uint16_t dataSize() noexcept {
		return this->blockSize - headerSize();
	}

	uint16_t getUsedSize() const noexcept {
		return this->used;
	}

	uint64_t getfPos() const noexcept {
		return this->currentfPos;
	}
	void setNextfpos(uint64_t fpos) noexcept {
		this->nextfpos = fpos;
	}
	uint64_t getNextPos() const noexcept {
		return this->nextfpos;
	}

	const char* getData() const noexcept {
		return this->data;
	}


private:
	static uint16_t toBlockSize(uint16_t used, uint16_t blockunit) noexcept;

private:
	uint16_t blockSize;
	uint64_t currentfPos;

	// header
	uint64_t nextfpos;
	uint16_t used;

	// body
	char* data;
};

} /* namespace alinous */

#endif /* FILESTORE_VARIABLE_BLOCK_VARIABLEBLOCK_H_ */
