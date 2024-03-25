/*
 * PoWCalculator.h
 *
 *  Created on: 2023/03/22
 *      Author: iizuka
 */

#ifndef POW_POWCALCULATOR_H_
#define POW_POWCALCULATOR_H_

#include "pow/AbstractPoWCalculator.h"

#include <cstdint>

namespace alinous {
class SystemTimestamp;
}
using namespace alinous;

namespace codablecash {

class PoWCalculatorThread;
class ISystemLogger;
class BlockMerkleRoot;

class PoWCalculator : public AbstractPoWCalculator {
public:
	PoWCalculator();
	virtual ~PoWCalculator();

	virtual void calculate(uint64_t lastBlockHeight, const BlockHeaderId* lastBlockId, const BlockMerkleRoot* merkleRoot
			, const SystemTimestamp* tm, const BigInteger* targetDifficulty);
	virtual void init(ISystemLogger* logger);
	virtual void shutdown();

	virtual void suspend();
	virtual void resume();

	virtual bool isSuspendStatus() const noexcept;


private:
	PoWCalculatorThread* thread;
};

} /* namespace codablecash */

#endif /* POW_POWCALCULATOR_H_ */
