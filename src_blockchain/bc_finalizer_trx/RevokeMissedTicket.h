/*
 * RevokeMissedTicket.h
 *
 *  Created on: 2023/05/17
 *      Author: iizuka
 */

#ifndef BC_FINALIZER_TRX_REVOKEMISSEDTICKET_H_
#define BC_FINALIZER_TRX_REVOKEMISSEDTICKET_H_

#include "bc_finalizer_trx/AbstractRevokeTransaction.h"

namespace codablecash {

class TicketUtxoReference;

class RevokeMissedTicket : public AbstractRevokeTransaction {
public:
	RevokeMissedTicket(const RevokeMissedTicket& inst);
	RevokeMissedTicket();
	virtual ~RevokeMissedTicket();

	virtual uint8_t getType() const noexcept {
		return AbstractBlockchainTransaction::TRX_TYPE_REVOKE_MISSED_TICKET;
	}

	virtual void fromBinary(ByteBuffer* in);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual IBlockObject* copyData() const noexcept;

	virtual void build();

	virtual bool validateOnAccept(MemPoolTransaction *memTrx, IStatusCacheContext* context) const;
	virtual TrxValidationResult validateFinal(const BlockHeader* header, MemPoolTransaction *memTrx, IStatusCacheContext* context) const;

	virtual AbstractUtxoReference* getUtxoReference(int i) const noexcept;
	virtual int getUtxoReferenceSize() const noexcept;

	void setTicketUtxoReference(const TicketUtxoReference* ref);
	TicketUtxoReference* getTicketUtxoReference() const noexcept {
		return ticketUtxoRef;
	}

private:
	TicketUtxoReference* ticketUtxoRef;
};

} /* namespace codablecash */

#endif /* BC_FINALIZER_TRX_REVOKEMISSEDTICKET_H_ */
