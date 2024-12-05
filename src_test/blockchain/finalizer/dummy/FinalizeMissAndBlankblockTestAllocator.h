/*
 * FinalizeMissTestAllocator.h
 *
 *  Created on: 2023/05/24
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_FINALIZER_DUMMY_FINALIZEMISSANDBLANKBLOCKTESTALLOCATOR_H_
#define BLOCKCHAIN_FINALIZER_DUMMY_FINALIZEMISSANDBLANKBLOCKTESTALLOCATOR_H_

#include "BlankBlockGeneratorAllocator.h"

namespace codablecash {

class FinalizeMissAndBlankblockTestAllocator : public BlankBlockGeneratorAllocator {
public:
	FinalizeMissAndBlankblockTestAllocator();
	virtual ~FinalizeMissAndBlankblockTestAllocator();
	virtual FinalizerPool* newFinalizerPool(const codablecash::CodablecashSystemParam *config, MemoryPool *memoryPool,
			BlockchainController *ctrl, P2pRequestProcessor* p2pRequestProcessor, BlochchainP2pManager* p2pManager, ISystemLogger *logger);
	virtual IInstanceMemberAllocator* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_FINALIZER_DUMMY_FINALIZEMISSANDBLANKBLOCKTESTALLOCATOR_H_ */
