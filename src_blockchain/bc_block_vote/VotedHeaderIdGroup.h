/*
 * VotedHeaderIdGroup.h
 *
 *  Created on: 2023/05/06
 *      Author: iizuka
 */

#ifndef BC_BLOCK_VOTE_VOTEDHEADERIDGROUP_H_
#define BC_BLOCK_VOTE_VOTEDHEADERIDGROUP_H_

#include "base/ArrayList.h"

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class BlockHeaderId;
class TransactionId;
class Block;
class IVoteTransactionIdCertificateBuilder;
class IVoteTransactionIdCertificatevisitor;

class VotedHeaderIdGroup {
public:
	explicit VotedHeaderIdGroup(const BlockHeaderId* headerId);
	virtual ~VotedHeaderIdGroup();

	void add(const TransactionId* trxId) noexcept;

	int binarySize() const;
	void toBinary(ByteBuffer* out) const;
	static VotedHeaderIdGroup* createFromBinary(ByteBuffer* in);

	const BlockHeaderId* getBlockHeaderId() const noexcept {
		return this->votedHeaderId;
	}

	int size() const noexcept {
		return this->trxIdList->size();
	}
	const TransactionId* get(int i) const noexcept {
		return this->trxIdList->get(i);
	}

	void buildVoteTransactionIdCertificate(const Block* block, IVoteTransactionIdCertificateBuilder* callback) const;
	void visitVoteTransactionIdCertificate(IVoteTransactionIdCertificatevisitor* visitor) const;

private:
	BlockHeaderId* votedHeaderId;

	ArrayList<TransactionId>* trxIdList;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_VOTE_VOTEDHEADERIDGROUP_H_ */
