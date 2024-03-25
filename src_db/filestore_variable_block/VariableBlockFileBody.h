/*
 * VariableBlockFileBody.h
 *
 *  Created on: 2023/02/08
 *      Author: iizuka
 */

#ifndef FILESTORE_VARIABLE_BLOCK_VARIABLEBLOCKFILEBODY_H_
#define FILESTORE_VARIABLE_BLOCK_VARIABLEBLOCKFILEBODY_H_

#include <cstdint>

namespace alinous {

class RandomAccessFile;

class VariableBlockFileBody {
public:
	explicit VariableBlockFileBody(RandomAccessFile* file, uint64_t blockSize);
	virtual ~VariableBlockFileBody();

	void createStore(bool del, uint64_t blockUnitSize);
	void sync(bool fileSync);
	void extend(uint64_t newLength);

	RandomAccessFile* getFile() const noexcept {
		return this->file;
	}

	void resetHeader(uint64_t fpos);

private:
	RandomAccessFile* file;
	uint64_t blockUnitSize;
};

} /* namespace alinous */

#endif /* FILESTORE_VARIABLE_BLOCK_VARIABLEBLOCKFILEBODY_H_ */
