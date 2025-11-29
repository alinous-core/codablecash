/*
 * InstanceIdIndexData.h
 *
 *  Created on: Nov 28, 2025
 *      Author: iizuka
 */

#ifndef SMARTCONTRACT_EXECUTOR_INDEX_INSTANCEIDINDEXDATA_H_
#define SMARTCONTRACT_EXECUTOR_INDEX_INSTANCEIDINDEXDATA_H_

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class CdbDatabaseSessionId;

class InstanceIdIndexData : public IBlockObject {
public:
	InstanceIdIndexData(const InstanceIdIndexData& inst);
	InstanceIdIndexData();
	InstanceIdIndexData(CdbDatabaseSessionId* id);

	virtual ~InstanceIdIndexData();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static InstanceIdIndexData* createFromBinary(ByteBuffer *in);

	virtual IBlockObject* copyData() const noexcept;

private:
	CdbDatabaseSessionId* trxId; // = SessionId
};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_EXECUTOR_INDEX_INSTANCEIDINDEXDATA_H_ */
