/*
 * VoteTicket.h
 *
 *  Created on: 2023/04/27
 *      Author: iizuka
 */

#ifndef BC_FINALIZER_VOTETICKET_H_
#define BC_FINALIZER_VOTETICKET_H_

#include <cstdint>

#include "bc_base/BalanceUnit.h"
namespace codablecash {

class NodeIdentifier;
class AddressDescriptor;
class TicketUtxo;
class UtxoId;

class VoteTicket {
public:
	VoteTicket(const VoteTicket& inst);
	VoteTicket();
	virtual ~VoteTicket();

	int binarySize() const;
	void toBinary(ByteBuffer* out) const;
	static VoteTicket* createFromBinary(ByteBuffer* in);

	static VoteTicket* toVoteTicket(uint64_t height, const TicketUtxo* ticketUtxo, const AddressDescriptor* voterAddressDesc);

	void setHeight(uint64_t height) noexcept {
		this->height = height;
	}
	uint64_t getHeight() const noexcept {
		return this->height;
	}

	void setNodeIdentifier(const NodeIdentifier* nodeId) noexcept;
	void setUtxoId(const UtxoId* utxoId) noexcept;
	void setAddressDesc(const AddressDescriptor* addressDesc) noexcept;
	void setTicketPrice(BalanceUnit ticketPrice);

	const UtxoId* getUtxoId() const noexcept {
		return this->utxoId;
	}
	const NodeIdentifier* getNodeIdentifier() const noexcept {
		return this->nodeId;
	}
	const AddressDescriptor* getAddressDescriptor() const noexcept {
		return this->addressDesc;
	}
	BalanceUnit getTicketPrice() const noexcept {
		return this->ticketPrice;
	}

	void setVoterAddressDesc(const AddressDescriptor* voterAddressDesc);
	const AddressDescriptor* getVoterAddressDesc() const noexcept {
		return this->voterAddressDesc;
	}
private:
	uint64_t height;
	UtxoId* utxoId;
	NodeIdentifier* nodeId;
	AddressDescriptor* voterAddressDesc; // pool's address

	AddressDescriptor* addressDesc; // registered ticket address
	BalanceUnit ticketPrice;
};

} /* namespace codablecash */

#endif /* BC_FINALIZER_VOTETICKET_H_ */
