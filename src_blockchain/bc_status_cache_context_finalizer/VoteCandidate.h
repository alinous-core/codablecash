/*
 * VoteCandidate.h
 *
 *  Created on: 2023/04/28
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_CONTEXT_FINALIZER_VOTECANDIDATE_H_
#define BC_STATUS_CACHE_CONTEXT_FINALIZER_VOTECANDIDATE_H_

#include "filestore_block/IBlockObject.h"

#include "bc_base/BalanceUnit.h"

using namespace alinous;

namespace codablecash {

class UtxoId;
class NodeIdentifier;
class AddressDescriptor;

class VoteCandidate : public IBlockObject {
public:
	VoteCandidate(const VoteCandidate& inst);
	VoteCandidate(const UtxoId* utxoId, const NodeIdentifier* nodeId);
	virtual ~VoteCandidate();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static VoteCandidate* createFromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void setVoted(bool bl) noexcept {
		this->voted = bl;
	}
	bool isVoted() const noexcept {
		return this->voted;
	}
	const NodeIdentifier* getNodeIdentifier() const noexcept {
		return this->nodeId;
	}
	const UtxoId* getUtxoId() const noexcept {
		return this->utxoId;
	}

	void setAddressDescriptor(const AddressDescriptor* addressDesc) noexcept;
	const AddressDescriptor* getAddressDescriptor() const noexcept {
		return this->addressDesc;
	}

	void setTicketPrice(BalanceUnit ticketPrice) noexcept;
	BalanceUnit getTicletPrice() const noexcept {
		return this->ticketPrice;
	}

	void setVoterAddressDescriptor(const AddressDescriptor* voterAddressDesc) noexcept;
	AddressDescriptor* getVoterAddressDescriptor() const noexcept {
		return this->voterAddressDesc;
	}

private:
	UtxoId* utxoId;
	NodeIdentifier* nodeId;
	AddressDescriptor* voterAddressDesc; // voter pool address
	bool voted;

	AddressDescriptor* addressDesc; // ticket address
	BalanceUnit ticketPrice;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_CONTEXT_FINALIZER_VOTECANDIDATE_H_ */
