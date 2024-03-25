/*
 * FinalizeMissTestPool.h
 *
 *  Created on: 2023/05/24
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_FINALIZER_DUMMY_FINALIZEMISSTESTPOOL_H_
#define BLOCKCHAIN_FINALIZER_DUMMY_FINALIZEMISSTESTPOOL_H_

#include "bc_finalizer_pool/FinalizerPool.h"

namespace codablecash {

class FinalizeMissTestPool : public FinalizerPool {
public:
	FinalizeMissTestPool(const CodablecashConfig* config, MemoryPool* memoryPool, BlockchainController* ctrl
			, P2pRequestProcessor* p2pRequestProcessor, BlochchainP2pManager* p2pManager, ISystemLogger* logger);
	virtual ~FinalizeMissTestPool();

	virtual void requestCheckVoting(uint16_t zone, const BlockHeaderId *headerId, uint64_t height);

private:
	void requestCheckVoting01(uint16_t zone, const BlockHeaderId *headerId, uint64_t height);
	void requestCheckVoting02(uint16_t zone, const BlockHeaderId *headerId, uint64_t height);

private:
	uint64_t count;
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_FINALIZER_DUMMY_FINALIZEMISSTESTPOOL_H_ */
