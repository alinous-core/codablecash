/*
 * PoWCalculator.cpp
 *
 *  Created on: 2023/03/22
 *      Author: iizuka
 */

#include "pow/PoWCalculator.h"
#include "pow/PoWCalculatorThread.h"

#include "base/UnicodeString.h"


namespace codablecash {

PoWCalculator::PoWCalculator() {
	this->thread = nullptr;
}

PoWCalculator::~PoWCalculator() {
	shutdown();
}

void PoWCalculator::calculate(uint64_t lastBlockHeight, const BlockHeaderId *lastBlockId, const BlockMerkleRoot* merkleRoot, const SystemTimestamp* tm, const BigInteger *targetDifficulty) {
	if(this->thread != nullptr){
		this->thread->request(lastBlockHeight, lastBlockId, merkleRoot, tm, targetDifficulty);
	}
}

void PoWCalculator::init(ISystemLogger* logger) {
	UnicodeString name(L"PoW_Calc");
	this->thread = new PoWCalculatorThread(this->powManager, &name, logger);
	this->thread->setRunning(true);
	this->thread->start();
}

void PoWCalculator::shutdown() {
	if(this->thread != nullptr){
		this->thread->setRunning(false);
		this->thread->join();

		delete this->thread;
		this->thread = nullptr;
	}
}

void PoWCalculator::suspend() {
	this->thread->setSuspended(true);
}

void PoWCalculator::resume() {
	this->thread->setSuspended(false);
}

bool PoWCalculator::isSuspendStatus() const noexcept {
	return this->thread->isSuspendStatus();
}

} /* namespace codablecash */
