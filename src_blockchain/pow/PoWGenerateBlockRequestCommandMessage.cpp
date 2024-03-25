/*
 * PoWGenerateBlockRequestCommandMessage.cpp
 *
 *  Created on: 2023/05/22
 *      Author: iizuka
 */

#include "pow/PoWGenerateBlockRequestCommandMessage.h"
#include "pow/PoWManager.h"
#include "pow/PoWNonce.h"

#include "bc_block/BlockHeaderId.h"

#include "bc_block_generator/BlockGenerator.h"


namespace codablecash {

PoWGenerateBlockRequestCommandMessage::PoWGenerateBlockRequestCommandMessage(uint64_t lastBlockHeight, const BlockHeaderId *lastBlockHeaderId, const PoWNonce *nonce) {
	this->lastBlockHeight = lastBlockHeight;
	this->lastBlockHeaderId = dynamic_cast<BlockHeaderId*>(lastBlockHeaderId->copyData());
	this->nonce = dynamic_cast<PoWNonce*>(nonce->copyData());
}

PoWGenerateBlockRequestCommandMessage::~PoWGenerateBlockRequestCommandMessage() {
	delete this->lastBlockHeaderId;
	delete this->nonce;
}

void PoWGenerateBlockRequestCommandMessage::process(PoWManager *manager) {
	BlockGenerator* generator = manager->getBlockGenerator();
	generator->nonceCalculated(this->lastBlockHeight, this->lastBlockHeaderId, this->nonce);
}

} /* namespace codablecash */
