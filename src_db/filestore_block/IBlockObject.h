/*
 * IBlockObject.h
 *
 *  Created on: 2018/12/22
 *      Author: iizuka
 */

#ifndef FILESTORE_BLOCK_IBLOCKOBJECT_H_
#define FILESTORE_BLOCK_IBLOCKOBJECT_H_
#include <cstdint>

#include "base_io/ByteBuffer.h"
#include "base/UnicodeString.h"

namespace alinous {

class ByteBuffer;
class UnicodeString;

class IBlockObject {
public:
	IBlockObject(){}
	virtual ~IBlockObject(){};

	virtual int binarySize() const = 0;
	virtual void toBinary(ByteBuffer* out) const = 0;

	virtual IBlockObject* copyData() const noexcept = 0;
};

} /* namespace alinous */


#endif /* FILESTORE_BLOCK_IBLOCKOBJECT_H_ */
