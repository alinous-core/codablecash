/*
 * TicketUtxoReference.h
 *
 *  Created on: 2023/05/03
 *      Author: iizuka
 */

#ifndef BC_FINALIZER_TRX_TICKETUTXOREFERENCE_H_
#define BC_FINALIZER_TRX_TICKETUTXOREFERENCE_H_

#include "bc_trx/AbstractUtxoReference.h"

namespace codablecash {

class TicketUtxoReference : public AbstractUtxoReference {
public:
	TicketUtxoReference(const TicketUtxoReference& inst);
	TicketUtxoReference();
	virtual ~TicketUtxoReference();

	virtual uint8_t getType() const noexcept {
		return AbstractUtxoReference::UTXO_REF_TYPE_UTXO_TICKET;
	}

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_FINALIZER_TRX_TICKETUTXOREFERENCE_H_ */
