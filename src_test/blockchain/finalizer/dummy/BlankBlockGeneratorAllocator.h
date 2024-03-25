/*
 * BlankBlockGeneratorAllocator.h
 *
 *  Created on: 2023/05/19
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_FINALIZER_DUMMY_BLANKBLOCKGENERATORALLOCATOR_H_
#define BLOCKCHAIN_FINALIZER_DUMMY_BLANKBLOCKGENERATORALLOCATOR_H_

#include "bc/DefaultInstanceMemberAllocator.h"

namespace codablecash {

class BlankBlockGeneratorAllocator : public DefaultInstanceMemberAllocator {
public:
	BlankBlockGeneratorAllocator();
	virtual ~BlankBlockGeneratorAllocator();

	virtual IInstanceMemberAllocator* copy() const noexcept {
		return new BlankBlockGeneratorAllocator();
	}

	virtual BlockGenerator* newBlockGenerator(uint16_t zone, CodablecashConfig* config, MemoryPool* memoryPool
			, BlockchainController* ctrl, const MiningConfig *miningConfig, ISystemLogger* logger);

};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_FINALIZER_DUMMY_BLANKBLOCKGENERATORALLOCATOR_H_ */
