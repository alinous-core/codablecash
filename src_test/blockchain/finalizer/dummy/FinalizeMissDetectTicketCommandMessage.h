/*
 * FinalizeMissDetectVotingTicketCommandMessage.h
 *
 *  Created on: 2023/05/24
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_FINALIZER_DUMMY_FINALIZEMISSDETECTTICKETCOMMANDMESSAGE_H_
#define BLOCKCHAIN_FINALIZER_DUMMY_FINALIZEMISSDETECTTICKETCOMMANDMESSAGE_H_

#include "bc_finalizer_pool/DetectVotingTicketCommandMessage.h"

namespace codablecash {

class FinalizeMissDetectTicketCommandMessage : public DetectVotingTicketCommandMessage {
public:
	FinalizeMissDetectTicketCommandMessage(uint16_t zone, const BlockHeaderId *headerId, uint64_t height);
	virtual ~FinalizeMissDetectTicketCommandMessage();

	virtual void process(FinalizerPool* pool);

	virtual void putTransaction(const BlockHeader* header, const VoteCandidate* candidate, IStatusCacheContext* context
			, MemPoolTransaction* memTrx, const CodablecashConfig* config, FinalizerPool *pool);
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_FINALIZER_DUMMY_FINALIZEMISSDETECTTICKETCOMMANDMESSAGE_H_ */
