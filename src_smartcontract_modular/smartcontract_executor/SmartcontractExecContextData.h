/*
 * SmartcontractExecContextData.h
 *
 *  Created on: Nov 28, 2025
 *      Author: iizuka
 */

#ifndef SMARTCONTRACT_EXECUTOR_SMARTCONTRACTEXECCONTEXTDATA_H_
#define SMARTCONTRACT_EXECUTOR_SMARTCONTRACTEXECCONTEXTDATA_H_
#include <cstdint>

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class CdbDatabaseSessionId;
class BlockHeaderId;

class SmartcontractExecContextData : public IBlockObject {
public:
	SmartcontractExecContextData(const SmartcontractExecContextData& inst);
	SmartcontractExecContextData();
	virtual ~SmartcontractExecContextData();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static SmartcontractExecContextData* createFromBinary(ByteBuffer *in);

	virtual IBlockObject* copyData() const noexcept;

private:
	uint64_t height;

	uint64_t lastHeight;
	BlockHeaderId* lastBlockHeaderId;
	CdbDatabaseSessionId* lastTrxId; // = SessionId

	// snapshot
	bool finalized;

};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_EXECUTOR_SMARTCONTRACTEXECCONTEXTDATA_H_ */
