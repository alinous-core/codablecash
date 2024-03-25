/*
 * RevokeMissVotedTicket.h
 *
 *  Created on: 2023/05/17
 *      Author: iizuka
 */

#ifndef BC_FINALIZER_TRX_REVOKEMISSVOTEDTICKET_H_
#define BC_FINALIZER_TRX_REVOKEMISSVOTEDTICKET_H_

#include "bc_finalizer_trx/AbstractRevokeTransaction.h"


namespace codablecash {

class TicketVotedUtxoReference;

class RevokeMissVotedTicket : public AbstractRevokeTransaction {
public:
	RevokeMissVotedTicket(const RevokeMissVotedTicket& inst);
	RevokeMissVotedTicket();
	virtual ~RevokeMissVotedTicket();

	virtual uint8_t getType() const noexcept {
		return AbstractBlockchainTransaction::TRX_TYPE_REVOKE_MISS_VOTED_TICKET;
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

	void setTicketVotedUtxoReference(const TicketVotedUtxoReference* ref) noexcept;

	TicketVotedUtxoReference* getTicketVotedUtxoReference() const noexcept {
		return this->ticketVoteUtxoRef;
	}

private:
	TicketVotedUtxoReference* ticketVoteUtxoRef;
};

} /* namespace codablecash */

#endif /* BC_FINALIZER_TRX_REVOKEMISSVOTEDTICKET_H_ */
