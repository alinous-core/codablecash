/*
 * AbstractRevokeTransaction.h
 *
 *  Created on: 2023/05/17
 *      Author: iizuka
 */

#ifndef BC_FINALIZER_TRX_ABSTRACTREVOKETRANSACTION_H_
#define BC_FINALIZER_TRX_ABSTRACTREVOKETRANSACTION_H_

#include "bc_finalizer_trx/AbstractFinalizerTransaction.h"

namespace codablecash {

class AbstractRevokeTransaction : public AbstractFinalizerTransaction {
public:
	AbstractRevokeTransaction(const AbstractRevokeTransaction& inst);
	AbstractRevokeTransaction();
	virtual ~AbstractRevokeTransaction();

	virtual bool canAddMempool() const noexcept {
		return false;
	}

protected:
	void setUtxoNonce() noexcept;

	int __binarySize() const;
	void __toBinary(ByteBuffer* out) const;
};

} /* namespace codablecash */

#endif /* BC_FINALIZER_TRX_ABSTRACTREVOKETRANSACTION_H_ */
