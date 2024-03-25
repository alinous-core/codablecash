/*
 * DefaultInstanceMemberAllocator.h
 *
 *  Created on: 2023/05/18
 *      Author: iizuka
 */

#ifndef BC_DEFAULTINSTANCEMEMBERALLOCATOR_H_
#define BC_DEFAULTINSTANCEMEMBERALLOCATOR_H_


#include "bc/IInstanceMemberAllocator.h"

namespace codablecash {

class DefaultInstanceMemberAllocator : public IInstanceMemberAllocator {
public:
	DefaultInstanceMemberAllocator();
	virtual ~DefaultInstanceMemberAllocator();

	virtual IInstanceMemberAllocator* copy() const noexcept {
		return new DefaultInstanceMemberAllocator();
	}

	virtual BlockGenerator* newBlockGenerator(uint16_t zone, CodablecashConfig* config, MemoryPool* memoryPool, BlockchainController* ctrl, const MiningConfig *miningConfig, ISystemLogger* logger);
	virtual FinalizerPool* newFinalizerPool(const CodablecashConfig* config, MemoryPool* memoryPool, BlockchainController* ctrl
			, P2pRequestProcessor* p2pRequestProcessor, BlochchainP2pManager* p2pManager, ISystemLogger* logger);

};

} /* namespace codablecash */

#endif /* BC_DEFAULTINSTANCEMEMBERALLOCATOR_H_ */
