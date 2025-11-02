/*
 * RegisterVotePoolTransaction.h
 *
 *  Created on: 2023/04/15
 *      Author: iizuka
 */

#ifndef BC_FINALIZER_TRX_REGISTERVOTEPOOLTRANSACTION_H_
#define BC_FINALIZER_TRX_REGISTERVOTEPOOLTRANSACTION_H_

#include "bc_finalizer_trx/AbstractFinalizerTransaction.h"

namespace codablecash {

class NodeIdentifier;
class AddressDescriptor;
class SchnorrSignature;
class NodeIdentifierSource;

class RegisterVotePoolTransaction : public AbstractFinalizerTransaction {
public:
	RegisterVotePoolTransaction(const RegisterVotePoolTransaction& inst);

	RegisterVotePoolTransaction();
	explicit RegisterVotePoolTransaction(const NodeIdentifier* nodeId,  const AddressDescriptor* addressDesc);
	virtual ~RegisterVotePoolTransaction();

	virtual uint8_t getType() const noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;

	virtual void fromBinary(ByteBuffer* in);

	virtual void build();

	IBlockObject* copyData() const noexcept;

	const NodeIdentifier* getNodeId() const noexcept {
		return this->nodeId;
	}
	const AddressDescriptor* getAddressDesc() const noexcept {
		return this->addressDesc;
	}

	void voterSign(const NodeIdentifierSource* source) noexcept;
	bool verifyVoterSign() const noexcept;

	virtual bool validateOnAccept(MemPoolTransaction *memTrx, IStatusCacheContext* context) const;
	virtual TrxValidationResult validateFinal(const BlockHeader* header, MemPoolTransaction *memTrx, IStatusCacheContext* context) const;

	virtual void addInternalMerkleTreeElement(MerkleTree* tree);

private:
	NodeIdentifier* nodeId;
	AddressDescriptor* addressDesc; // voter address

	SchnorrSignature* voterSig;
};

} /* namespace codablecash */

#endif /* BC_FINALIZER_TRX_REGISTERVOTEPOOLTRANSACTION_H_ */
