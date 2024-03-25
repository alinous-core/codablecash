/*
 * RemovedDummyUtxo.h
 *
 *  Created on: 2023/05/12
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_CONTEXT_REMOVEDDUMMYUTXO_H_
#define BC_STATUS_CACHE_CONTEXT_REMOVEDDUMMYUTXO_H_

#include "bc_trx/AbstractUtxo.h"

namespace codablecash {


class RemovedDummyUtxo : public AbstractUtxo {
public:
	RemovedDummyUtxo(const RemovedDummyUtxo& inst);
	RemovedDummyUtxo();
	virtual ~RemovedDummyUtxo();

	virtual uint8_t getType() const noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;
	virtual void build();

	virtual const AddressDescriptor* getAddress() const noexcept;
	virtual BalanceUnit getAmount() const noexcept;

	void setUtxoId(const UtxoId* utxoId) noexcept;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_CONTEXT_REMOVEDDUMMYUTXO_H_ */
