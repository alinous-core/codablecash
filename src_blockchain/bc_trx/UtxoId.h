/*
 * UtxoId.h
 *
 *  Created on: 2023/02/23
 *      Author: iizuka
 */

#ifndef BC_TRX_UTXOID_H_
#define BC_TRX_UTXOID_H_

#include "bc_base/Abstract32BytesId.h"

namespace codablecash {

class UtxoId : public Abstract32BytesId {
public:
	UtxoId();
	UtxoId(const char* binary, int length);
	virtual ~UtxoId();

	static UtxoId* fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_TRX_UTXOID_H_ */
