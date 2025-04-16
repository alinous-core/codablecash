/*
 * ITransferedData.h
 *
 *  Created on: 2023/09/18
 *      Author: iizuka
 */

#ifndef DATA_HISTORY_ABSTRACTTRANSFEREDDATA_H_
#define DATA_HISTORY_ABSTRACTTRANSFEREDDATA_H_

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class TransferedDataId;

class AbstractTransferedData : public IBlockObject {
public:
	static const constexpr uint8_t DATA_TRANSACTION{1};
	static const constexpr uint8_t DATA_BLOCKHEADER{2};

	static const constexpr uint8_t DATA_LIVEDATA{10};

	AbstractTransferedData(const AbstractTransferedData& inst);
	explicit AbstractTransferedData(uint8_t type);
	virtual ~AbstractTransferedData();

	static AbstractTransferedData* createFromBinary(ByteBuffer* in);

	virtual void fromBinary(ByteBuffer* in) = 0;

	virtual TransferedDataId* getTransferedDataId() const = 0;

	uint8_t getType() const noexcept {
		return this->type;
	}

protected:
	uint8_t type;
};

} /* namespace codablecash */

#endif /* DATA_HISTORY_ABSTRACTTRANSFEREDDATA_H_ */
