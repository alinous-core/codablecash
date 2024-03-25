/*
 * ValidationInstance.cpp
 *
 *  Created on: 2023/12/03
 *      Author: iizuka
 */

#include "ValidationInstance.h"

#include "base_io/File.h"

#include "bc/CodablecashNodeInstance.h"

#include "bc/exceptions.h"

#include "bc_block/Block.h"

#include "bc_block_validator/BlockValidator.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc_block/BlockHeader.h"

#include "bc_block/BlockMerkleRoot.h"

#include "bc_block_body/BlockBody.h"

#include "base/StackRelease.h"
namespace codablecash {

ValidationInstance::ValidationInstance(const File* baseDir) {
	this->instDir = baseDir->get(L"validation");
	this->inst = nullptr;
}

ValidationInstance::~ValidationInstance() {
	if(this->inst != nullptr){
		this->inst->shutdown();
	}
	delete this->inst;

	delete this->instDir;
}

void ValidationInstance::initInstance(const CodablecashConfig *config, ISystemLogger *logger) {
	{
		CodablecashNodeInstance inst(this->instDir, logger, config);
		bool bl = inst.initBlankInstance(0, 1);
		if(!bl){
			throw new MulformattedTransactionBinaryException(__FILE__, __LINE__);
		}
	}

	this->inst = new CodablecashNodeInstance(this->instDir, logger, config);
	this->inst->load();
}

void ValidationInstance::validateBlock(const Block *block) {
	BlockchainController* ctrl = this->inst->getController();
	CodablecashConfig* config = this->inst->getCodablecashConfig();
	MemoryPool* memPool = this->inst->getMemoryPool();


	{
		BlockValidator validator(block, config, memPool, ctrl);
		validator.validate();
	}

	BlockHeader* header = block->getHeader();
	BlockBody* body = block->getBody();

	const BlockMerkleRoot* rootheader = header->getMerkleRoot();
	const BlockMerkleRoot* rootbody = body->getMerkleRoot();

	UnicodeString* str1 = rootheader->toString(); __STP(str1);
	UnicodeString* str2 = rootbody->toString(); __STP(str2);

	bool bl = rootheader->equals(rootbody);
	assert(bl == true);

	ctrl->addBlock(block);
}

} /* namespace codablecash */
