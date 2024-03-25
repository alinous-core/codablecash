/*
 * PoWCalculatorThread.h
 *
 *  Created on: 2023/03/22
 *      Author: iizuka
 */

#ifndef POW_POWCALCULATORTHREAD_H_
#define POW_POWCALCULATORTHREAD_H_

#include "base_thread/AbstractThreadRunner.h"

#include <cstdint>

namespace alinous {
class SynchronizedLock;
class BigInteger;
class SystemTimestamp;
}
using namespace alinous;

namespace codablecash {

class BlockHeaderId;
class PoWManager;
class PoWNonce;
class ISystemLogger;
class BlockMerkleRoot;

class PoWCalculatorThread : public AbstractThreadRunner {
public:
	static const constexpr int STAT_STOPPED{0};
	static const constexpr int STAT_WAITING{1};
	static const constexpr int STAT_RUNNING{2};


	PoWCalculatorThread(PoWManager* powManager, const UnicodeString* name, ISystemLogger* logger);
	virtual ~PoWCalculatorThread();

	void setRunning(bool bl) noexcept;
	bool isRunning() const noexcept;

	void setSuspended(bool bl) noexcept;
	bool isSuspend() const noexcept;

	bool isSuspendStatus() const noexcept;

	void request(uint64_t lastBlockHeight, const BlockHeaderId *lastBlockId, const BlockMerkleRoot* merkleRoot, const SystemTimestamp* tm, const BigInteger *targetDifficulty) noexcept;

protected:
	virtual void process() noexcept;

private:
	void doCalc(uint64_t height, int maxLoop, const BlockHeaderId* blockHeaderId, const BigInteger* tdiff, const BlockMerkleRoot* merkleRoot, const SystemTimestamp* tm);
	PoWNonce* calcOne(const BlockHeaderId* blockHeaderId, const BigInteger* tdiff, const BlockMerkleRoot* merkleRoot, const SystemTimestamp* tm);

	void checkAndReportNonce(uint64_t height, const BlockHeaderId* bid, const BigInteger* tdiff, const PoWNonce* nonce);

private:
	SynchronizedLock* lock;
	volatile bool running;
	volatile bool suspended;
	volatile int status;

	BlockHeaderId* lastBlockId;
	BigInteger* targetDifficulty;
	uint64_t lastBlockHeight;

	BlockMerkleRoot* merkleRoot;
	SystemTimestamp* timestamp;

	PoWManager* powManager;

	ISystemLogger* logger;
};

} /* namespace codablecash */

#endif /* POW_POWCALCULATORTHREAD_H_ */
