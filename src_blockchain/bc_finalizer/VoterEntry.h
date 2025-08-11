/*
 * VoterEntry.h
 *
 *  Created on: 2023/04/15
 *      Author: iizuka
 */

#ifndef BC_FINALIZER_VOTERENTRY_H_
#define BC_FINALIZER_VOTERENTRY_H_

#include "base/ArrayList.h"

#include <cstdint>

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class NodeIdentifier;
class VoteTicket;
class AddressDescriptor;
class UtxoId;
class BalanceUnit;
class MerkleCertificate;

class VoterEntry : public IBlockObject {
public:
	VoterEntry(const VoterEntry& inst);
	VoterEntry();
	virtual ~VoterEntry();

	static VoterEntry* createVoteEntry(const NodeIdentifier* nodeId, const AddressDescriptor* voterAddress);

	const NodeIdentifier* getNodeIdentifier() const noexcept {
		return this->nodeId;
	}
	const AddressDescriptor* getVoterAddressDescriptor() const noexcept {
		return this->voterAddressDesc;
	}

	void setNodeIdentifier(const NodeIdentifier* nodeId) noexcept;
	void setCapacity(int cap);
	void setAddressDesc(const AddressDescriptor* addressDesc) noexcept;

	void addTicket(const VoteTicket* ticket) noexcept;
	bool removeTicket(const UtxoId* utxoId) noexcept;

	const VoteTicket* getTicket(const UtxoId* utxoId) const;

	struct VoteCompare {
	public:
		int operator() (const VoterEntry* const a, const VoterEntry* const b) const noexcept;
	};

	void setPosition(int pos) noexcept {
		this->pos = pos;
	}

	const VoteTicket* nextTicket(uint64_t matureHeight) noexcept;

	void handleVoted(int maxextendCount) noexcept;
	void handleMissed(int missingLimit) noexcept;

	bool isUpdated() const noexcept {
		return this->updated;
	}
	void setUpdated(bool bl) noexcept {
		this->updated = bl;
	}

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static VoterEntry* createFromBinary(ByteBuffer* in);
	virtual IBlockObject* copyData() const noexcept;

	BalanceUnit getTicketPriceSum() const noexcept;

	void setNodeIdMerkleCert(const MerkleCertificate* cert) noexcept;
	const MerkleCertificate* getNodeIdMerkleCertificate() const noexcept {
		return this->nodeIdMerkleCert;
	}

#ifdef __DEBUG__
	bool checkTickets() const;
	int countTicket(const UtxoId* id) const;
#endif

private:
	NodeIdentifier* nodeId;
	AddressDescriptor* voterAddressDesc;

	int missingCount;
	int extendCount;
	int capacity;

	ArrayList<VoteTicket>* list;

	int pos;
	bool updated;

	MerkleCertificate* nodeIdMerkleCert;
};

} /* namespace codablecash */

#endif /* BC_FINALIZER_VOTERENTRY_H_ */
