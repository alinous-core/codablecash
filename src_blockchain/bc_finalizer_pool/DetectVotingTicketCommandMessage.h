/*
 * DetectVotingTicket.h
 *
 *  Created on: 2023/05/03
 *      Author: iizuka
 */

#ifndef BC_FINALIZER_POOL_DETECTVOTINGTICKETCOMMANDMESSAGE_H_
#define BC_FINALIZER_POOL_DETECTVOTINGTICKETCOMMANDMESSAGE_H_

#include "bc_finalizer_pool/AbstractFinalizerCommandMessage.h"

#include <cstdint>

namespace codablecash {

class BlockHeaderId;
class BlockHeader;
class BlockHeaderStoreManager;
class VoteCandidate;
class IStatusCacheContext;
class CodablecashSystemParam;
class MemPoolTransaction;

class DetectVotingTicketCommandMessage : public AbstractFinalizerCommandMessage {
public:
	DetectVotingTicketCommandMessage(uint16_t zone, const BlockHeaderId *headerId, uint64_t height);
	virtual ~DetectVotingTicketCommandMessage();

protected:
	virtual void process(FinalizerPool* pool);

	virtual void putTransaction(const BlockHeader* header, const VoteCandidate* candidate, IStatusCacheContext* context
			, MemPoolTransaction* memTrx, const CodablecashSystemParam* config, FinalizerPool *pool);
	BlockHeader* getNblocksBefore(const BlockHeaderId* headerId, int voteBeforeNBlocks, BlockHeaderStoreManager* headerManager);

protected:
	uint16_t zone;
	BlockHeaderId *headerId;
	uint64_t height;
};

} /* namespace codablecash */

#endif /* BC_FINALIZER_POOL_DETECTVOTINGTICKETCOMMANDMESSAGE_H_ */
