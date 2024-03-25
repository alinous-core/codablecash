/*
 * FilePositionData.h
 *
 *  Created on: 2023/03/02
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_BODY_FILEPOSITIONDATA_H_
#define BC_BLOCKSTORE_BODY_FILEPOSITIONDATA_H_

#include <cstdint>

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class FilePositionData : public IBlockObject {
public:
	explicit FilePositionData(uint64_t fpos);
	virtual ~FilePositionData();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static FilePositionData* createFromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	uint64_t getFpos() const noexcept {
		return this->fpos;
	}

private:
	uint64_t fpos;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_BODY_FILEPOSITIONDATA_H_ */
