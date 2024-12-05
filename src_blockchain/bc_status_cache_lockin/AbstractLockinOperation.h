/*
 * AbstractLockonOperation.h
 *
 *  Created on: 2024/02/11
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_LOCKIN_ABSTRACTLOCKINOPERATION_H_
#define BC_STATUS_CACHE_LOCKIN_ABSTRACTLOCKINOPERATION_H_

#include <cstdint>

#include "filestore_block/IBlockObject.h"

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class IStatusCacheContext;
class BlockHeader;

class AbstractLockinOperation : public IBlockObject {
public:
	const constexpr static uint16_t TYPE_VOTED{1};
	const constexpr static uint16_t TYPE_MISS_VOTED{2};

	AbstractLockinOperation(const AbstractLockinOperation& inst);
	explicit AbstractLockinOperation(uint16_t type);
	virtual ~AbstractLockinOperation();

	static AbstractLockinOperation* createFromBinary(ByteBuffer* in);

	virtual int binarySize() const = 0;
	virtual void toBinary(ByteBuffer* out) const = 0;
	virtual void fromBinary(ByteBuffer* in) = 0;

	virtual void apply(const BlockHeader *header, IStatusCacheContext* context) = 0;

	virtual IBlockObject* copyData() const noexcept = 0;

	virtual bool equals(const AbstractLockinOperation* op) const noexcept;

protected:
	virtual bool dataEquals(const AbstractLockinOperation* op) const noexcept = 0;

protected:
	uint16_t type;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_LOCKIN_ABSTRACTLOCKINOPERATION_H_ */
