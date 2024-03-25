/*
 * VariableBlockHandle.h
 *
 *  Created on: 2023/02/08
 *      Author: iizuka
 */

#ifndef FILESTORE_VARIABLE_BLOCK_VARIABLEBLOCKHANDLE_H_
#define FILESTORE_VARIABLE_BLOCK_VARIABLEBLOCKHANDLE_H_

#include "filestore_block/IBlockHandle.h"

#include "base/ArrayList.h"

namespace alinous {

class VariableBlockFileStore;
class VariableBlock;

class VariableBlockHandle : public IBlockHandle {
public:
	explicit VariableBlockHandle(VariableBlockFileStore* store);
	virtual ~VariableBlockHandle();

	virtual uint64_t getFpos() const;
	virtual void write(const char* bytes, int length);
	virtual ByteBuffer* getBuffer() const;

	virtual void removeBlocks();
	void removeBlocks(ArrayList<VariableBlock>* list);
	virtual int size() const noexcept;

	void setBuffer(ByteBuffer* buffer);
	ByteBuffer* moveBuffer() noexcept;

	void setFpos(uint64_t fpos) noexcept {
		this->fpos = fpos;
	}

private:
	bool needRealloc(ArrayList<VariableBlock>* list, uint64_t length);
	ArrayList<VariableBlock>* realloc(uint64_t length);

	uint64_t getAvailableSize(ArrayList<VariableBlock>* list );

private:
	uint64_t fpos;
	VariableBlockFileStore* store;
	ByteBuffer* buffer;
};

} /* namespace alinous */

#endif /* FILESTORE_VARIABLE_BLOCK_VARIABLEBLOCKHANDLE_H_ */
