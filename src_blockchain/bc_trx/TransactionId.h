/*
 * TransactionId.h
 *
 *  Created on: 2019/01/05
 *      Author: iizuka
 */

#ifndef BC_BASE_TRANSACTIONID_H_
#define BC_BASE_TRANSACTIONID_H_

#include "bc_base/Abstract32BytesId.h"

namespace alinous {
class ByteBuffer;
}

namespace codablecash {
using namespace alinous;

class TransactionId : public Abstract32BytesId {
protected:
	TransactionId();
public:
	explicit TransactionId(const TransactionId& inst);
	TransactionId(const char* binary, int length);
	virtual ~TransactionId();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static TransactionId* fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	struct ValueCompare {
	public:
		int operator() (const TransactionId* const a, const TransactionId* const b) const noexcept;
	};

	int hashCode() const;
};

} /* namespace codablecash */

#endif /* BC_BASE_TRANSACTIONID_H_ */
