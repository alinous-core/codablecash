/*
 * LockInOperationData.h
 *
 *  Created on: Oct 18, 2024
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_LOCKIN_LOCKINOPERATIONSDATA_H_
#define BC_STATUS_CACHE_LOCKIN_LOCKINOPERATIONSDATA_H_

#include "filestore_block/IBlockObject.h"

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class AbstractLockinOperation;
class BlockHeader;
class IStatusCacheContext;

class LockInOperationsData : public IBlockObject {
public:
	LockInOperationsData();
	virtual ~LockInOperationsData();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static LockInOperationsData* fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void add(const AbstractLockinOperation* op);

	void join(const LockInOperationsData* value) noexcept;
	bool contains(const AbstractLockinOperation* value) const noexcept;

	void apply(const BlockHeader *header, IStatusCacheContext* context);

private:
	ArrayList<AbstractLockinOperation> list;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_LOCKIN_LOCKINOPERATIONSDATA_H_ */
