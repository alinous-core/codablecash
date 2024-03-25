/*
 * FinalizeMissTestAllocator.h
 *
 *  Created on: 2023/12/03
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_FINALIZER_DUMMY_FINALIZEMISSTESTALLOCATOR_H_
#define BLOCKCHAIN_FINALIZER_DUMMY_FINALIZEMISSTESTALLOCATOR_H_

#include "bc/DefaultInstanceMemberAllocator.h"

namespace codablecash {

class FinalizeMissTestAllocator : public DefaultInstanceMemberAllocator {
public:
	FinalizeMissTestAllocator();
	virtual ~FinalizeMissTestAllocator();

	virtual BlockGenerator* newBlockGenerator(uint16_t zone, CodablecashConfig* config, MemoryPool* memoryPool
			, BlockchainController* ctrl, const MiningConfig *miningConfig, ISystemLogger* logger);

	virtual FinalizerPool* newFinalizerPool(const codablecash::CodablecashConfig *config, MemoryPool *memoryPool,
			BlockchainController *ctrl, P2pRequestProcessor* p2pRequestProcessor, BlochchainP2pManager* p2pManager, ISystemLogger *logger);
	virtual IInstanceMemberAllocator* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_FINALIZER_DUMMY_FINALIZEMISSTESTALLOCATOR_H_ */
