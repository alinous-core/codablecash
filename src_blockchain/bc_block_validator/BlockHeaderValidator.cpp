/*
 * BlockHeaderValidator.cpp
 *
 *  Created on: Jun 27, 2026
 *      Author: iizuka
 */

#include "bc_block_validator/BlockHeaderValidator.h"
#include "bc_block_validator/BlockValidationException.h"

#include "bc_block/BlockHeader.h"

#include "bc/ExceptionThrower.h"

#include "bc_status_cache/BlockchainController.h"

#include "base/StackRelease.h"

#include "base_timestamp/SystemTimestamp.h"

#include "pow/PoWNonce.h"
#include "pow/PoWNonceResult.h"

#include "numeric/BigInteger.h"


namespace codablecash {

BlockHeaderValidator::BlockHeaderValidator(const BlockHeader* blockHeader, CodablecashSystemParam* config, MemoryPool* memoryPool, BlockchainController* ctrl) {
	this->blockHeader = blockHeader;
	this->config = config;
	this->memoryPool = memoryPool;
	this->ctrl = ctrl;
}

BlockHeaderValidator::~BlockHeaderValidator() {

}

void BlockHeaderValidator::validate() {
	// validate last header
	validateLastHeader();

	validateHashrate();
}

void BlockHeaderValidator::validateLastHeader() {
	if(this->blockHeader->getHeight() == 1){
		return;
	}

	uint16_t zone = this->blockHeader->getZone();
	uint16_t numZones = this->ctrl->getNUmZones();
	ExceptionThrower<BlockValidationException>::throwExceptionIfCondition(zone >= numZones
			, L"The block zone does not exists.", __FILE__, __LINE__);

	const BlockHeaderId *lastHeaderId = this->blockHeader->getLastHeaderId();
	uint64_t lastBlockHeight = this->blockHeader->getHeight() - 1;

	BlockHeader* lastBlock = this->ctrl->getBlockHeaderHeightAt(zone, lastBlockHeight, lastHeaderId); __STP(lastBlock);
	ExceptionThrower<BlockValidationException>::throwExceptionIfCondition(lastBlock == nullptr
			, L"Invalid last block id.", __FILE__, __LINE__);

	const SystemTimestamp* nonceTm = this->blockHeader->getNonceGeneratedtimestamp();
	const SystemTimestamp* tm = this->blockHeader->getTimestamp();
	ExceptionThrower<BlockValidationException>::throwExceptionIfCondition(tm->compareTo(nonceTm) > 0
			, L"The block Timestamp must be before nonce calculated time.", __FILE__, __LINE__);
}

void BlockHeaderValidator::validateHashrate() {
	if(this->blockHeader->getHeight() == 1){
		return;
	}

	const PoWNonce* nonce = this->blockHeader->getPoWNonce();
	PoWNonceResult* res = nonce->calcResult(this->blockHeader->getLastHeaderId(), this->blockHeader->getMerkleRoot(), this->blockHeader->getTimestamp()); __STP(res);
	BigInteger diff = res->getDifficulty();

	uint16_t zone = this->blockHeader->getZone();
	uint64_t lastHeight = this->blockHeader->getHeight() - 1;
	const BlockHeaderId *lastHeaderId = this->blockHeader->getLastHeaderId();
	const SystemTimestamp* tm = this->blockHeader->getTimestamp();

	BigInteger targetDiff = this->ctrl->calcTargetDifficulty(zone, lastHeight, lastHeaderId, tm);

	ExceptionThrower<BlockValidationException>::throwExceptionIfCondition(diff.compareTo(targetDiff) < 0
			, L"Difficulty of the hash is not enough.", __FILE__, __LINE__);
}

} /* namespace codablecash */
