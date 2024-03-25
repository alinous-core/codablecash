/*
 * BlockHeaderTransferDataValidator.h
 *
 *  Created on: 2023/11/12
 *      Author: iizuka
 */

#ifndef DATA_HISTORY_DATA_BLOCKHEADERTRANSFERDATAVALIDATOR_H_
#define DATA_HISTORY_DATA_BLOCKHEADERTRANSFERDATAVALIDATOR_H_

#include "bc_block/BlockHeader.h"

namespace codablecash {

class BlockHeaderTransferData;

class BlockHeaderTransferDataValidator : public IVoteTransactionIdCertificatevisitor {
public:
	explicit BlockHeaderTransferDataValidator(const BlockHeaderTransferData* data);
	virtual ~BlockHeaderTransferDataValidator();

	virtual void visit(const BlockHeaderId* votedHeaderId, const TransactionId* voteTrxId);

private:
	const BlockHeaderTransferData* data;
};

} /* namespace codablecash */

#endif /* DATA_HISTORY_DATA_BLOCKHEADERTRANSFERDATAVALIDATOR_H_ */
