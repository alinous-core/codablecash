/*
 * BlockHeaderValidator.h
 *
 *  Created on: Jun 27, 2026
 *      Author: iizuka
 */

#ifndef BC_BLOCK_VALIDATOR_BLOCKHEADERVALIDATOR_H_
#define BC_BLOCK_VALIDATOR_BLOCKHEADERVALIDATOR_H_

namespace codablecash {

class BlockHeader;
class CodablecashSystemParam;
class MemoryPool;
class BlockchainController;


class BlockHeaderValidator {
public:
	BlockHeaderValidator(const BlockHeader* blockHeader, CodablecashSystemParam* config, MemoryPool* memoryPool, BlockchainController* ctrl);
	virtual ~BlockHeaderValidator();

	void validate();

private:
	void validateLastHeader();
	void validateHashrate();

private:
	const BlockHeader* blockHeader;

	CodablecashSystemParam* config;
	MemoryPool* memoryPool;
	BlockchainController* ctrl;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_VALIDATOR_BLOCKHEADERVALIDATOR_H_ */
