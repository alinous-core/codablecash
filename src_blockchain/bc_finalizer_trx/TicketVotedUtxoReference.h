/*
 * TicketVotedUtxoReference.h
 *
 *  Created on: 2023/05/15
 *      Author: iizuka
 */

#ifndef BC_FINALIZER_TRX_TICKETVOTEDUTXOREFERENCE_H_
#define BC_FINALIZER_TRX_TICKETVOTEDUTXOREFERENCE_H_

#include "bc_trx/AbstractUtxoReference.h"

namespace codablecash {

class TicketVotedUtxoReference : public AbstractUtxoReference {
public:
	TicketVotedUtxoReference(const TicketVotedUtxoReference& inst);
	TicketVotedUtxoReference();
	virtual ~TicketVotedUtxoReference();

	virtual uint8_t getType() const noexcept {
		return AbstractUtxoReference::UTXO_REF_TYPE_UTXO_VOTED_TICKET;
	}

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_FINALIZER_TRX_TICKETVOTEDUTXOREFERENCE_H_ */
