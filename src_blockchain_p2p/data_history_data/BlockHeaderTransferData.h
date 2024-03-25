/*
 * BlockHeaderTransferData.h
 *
 *  Created on: 2023/11/06
 *      Author: iizuka
 */

#ifndef DATA_HISTORY_DATA_BLOCKHEADERTRANSFERDATA_H_
#define DATA_HISTORY_DATA_BLOCKHEADERTRANSFERDATA_H_

#include "data_history/AbstractTransferedData.h"

#include "bc_block/BlockHeader.h"

#include "base/ArrayList.h"

namespace codablecash {

class BlockHeader;
class VoteTransactionIdCertificate;
class Block;

class BlockHeaderTransferData : public AbstractTransferedData, public IVoteTransactionIdCertificateBuilder {
public:
	BlockHeaderTransferData(const BlockHeaderTransferData& inst);
	BlockHeaderTransferData();
	virtual ~BlockHeaderTransferData();

	static BlockHeaderTransferData* createFromBlock(const Block* block);

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual TransferedDataId* getTransferedDataId() const;

	void setHeader(const BlockHeader* header) noexcept;
	const BlockHeader* getHeader() const noexcept {
		return this->header;
	}
	void addCert(const VoteTransactionIdCertificate* cert) noexcept;

	virtual void detect(const Block* block, BlockHeaderId* votedHeaderId, TransactionId* voteTrxId);

	void validateVoteTransactionsData() const;

	const VoteTransactionIdCertificate* getVoteTransactionIdCertificate(const TransactionId *voteTrxId) const;

private:
	BlockHeader* header;
	ArrayList<VoteTransactionIdCertificate>* certList;
};

} /* namespace codablecash */

#endif /* DATA_HISTORY_DATA_BLOCKHEADERTRANSFERDATA_H_ */
