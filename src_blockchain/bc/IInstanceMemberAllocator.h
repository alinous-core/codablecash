/*
 * IInstanceMemberAllocator.h
 *
 *  Created on: 2023/05/18
 *      Author: iizuka
 */

#ifndef BC_IINSTANCEMEMBERALLOCATOR_H_
#define BC_IINSTANCEMEMBERALLOCATOR_H_
#include <cstdint>

namespace codablecash {

class CodablecashConfig;
class MemoryPool;
class BlockchainController;
class MiningConfig;
class BlockGenerator;
class FinalizerPool;
class ISystemLogger;
class P2pRequestProcessor;
class BlochchainP2pManager;

class IInstanceMemberAllocator {
public:
	IInstanceMemberAllocator();
	virtual ~IInstanceMemberAllocator();

	virtual IInstanceMemberAllocator* copy() const noexcept = 0;

	virtual BlockGenerator* newBlockGenerator(uint16_t zone, CodablecashConfig* config, MemoryPool* memoryPool, BlockchainController* ctrl
			, const MiningConfig *miningConfig, ISystemLogger* logger) = 0;
	virtual FinalizerPool* newFinalizerPool(const CodablecashConfig* config, MemoryPool* memoryPool, BlockchainController* ctrl
			, P2pRequestProcessor* p2pRequestProcessor, BlochchainP2pManager* p2pManager, ISystemLogger* logger) = 0;
};

} /* namespace codablecash */

#endif /* BC_IINSTANCEMEMBERALLOCATOR_H_ */
