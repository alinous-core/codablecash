/*
 * RegisterTicketTransaction.h
 *
 *  Created on: 2023/04/15
 *      Author: iizuka
 */

#ifndef BC_FINALIZER_TRX_REGISTERTICKETTRANSACTION_H_
#define BC_FINALIZER_TRX_REGISTERTICKETTRANSACTION_H_

#include "bc_finalizer_trx/AbstractFinalizerTransaction.h"

namespace codablecash {

class NodeIdentifier;
class TicketUtxo;
class AddressDescriptor;

class RegisterTicketTransaction : public AbstractFinalizerTransaction {
public:
	RegisterTicketTransaction(const RegisterTicketTransaction& inst);
	RegisterTicketTransaction();
	virtual ~RegisterTicketTransaction();

	void setNodeId(const NodeIdentifier* nodeId) noexcept;
	void setAddressDescriptor(const AddressDescriptor* ticketReturnaddressDesc) noexcept;
	void setAmounst(BalanceUnit amount) noexcept;

	virtual bool validateOnAccept(MemPoolTransaction *memTrx, IStatusCacheContext* context) const;
	virtual TrxValidationResult validateFinal(const BlockHeader* header, MemPoolTransaction *memTrx, IStatusCacheContext* context) const;

	virtual uint8_t getType() const noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;

	virtual void fromBinary(ByteBuffer* in);

	virtual void build();
	virtual IBlockObject* copyData() const noexcept;

	virtual int getUtxoSize() const noexcept;
	virtual AbstractUtxo* getUtxo(int i) const noexcept;

	TicketUtxo* getTicketUtxo() const noexcept {
		return this->ticketUtxo;
	}

private:
	TicketUtxo* ticketUtxo;
};

} /* namespace codablecash */

#endif /* BC_FINALIZER_TRX_REGISTERTICKETTRANSACTION_H_ */
