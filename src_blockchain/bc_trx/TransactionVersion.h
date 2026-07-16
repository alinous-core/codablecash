/*
 * TransactionVersion.h
 *
 *  Created on: May 6, 2026
 *      Author: iizuka
 */

#ifndef BC_TRX_TRANSACTIONVERSION_H_
#define BC_TRX_TRANSACTIONVERSION_H_

#include "bc/SoftwareVersion.h"

namespace codablecash {

class TransactionVersion : public SoftwareVersion {
public:
	TransactionVersion(const TransactionVersion& inst);
	TransactionVersion(int major, int minor, int patch);
	virtual ~TransactionVersion();

	static TransactionVersion* createFromBinary(ByteBuffer* in);
};

} /* namespace codablecash */

#endif /* BC_TRX_TRANSACTIONVERSION_H_ */
