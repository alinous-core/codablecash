/*
 * TransferedDataId.h
 *
 *  Created on: 2023/09/18
 *      Author: iizuka
 */

#ifndef DATA_HISTORY_TRANSFEREDDATAID_H_
#define DATA_HISTORY_TRANSFEREDDATAID_H_

#include "bc_base/Abstract32BytesId.h"

namespace codablecash {

class TransferedDataId : public Abstract32BytesId {
public:
	TransferedDataId(const TransferedDataId& inst);
	TransferedDataId();
	TransferedDataId(const char* binary, int length);
	virtual ~TransferedDataId();

	static TransferedDataId* fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;
};

} /* namespace codablecash */

#endif /* DATA_HISTORY_TRANSFEREDDATAID_H_ */
