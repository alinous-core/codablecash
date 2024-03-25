/*
 * DownloadOmittedBlockBodyNodeCommand.cpp
 *
 *  Created on: 2023/11/24
 *      Author: iizuka
 */

#include "bc_p2p_cmd_node/DownloadOmittedBlockBodyNodeCommand.h"
#include "bc_p2p_cmd_node/DownloadOmittedBlockBodyNodeCommandResponse.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc/CodablecashNodeInstance.h"

#include "bc_block/Block.h"
#include "bc_block/BlockHeaderId.h"

#include "bc_block_body/BlockBody.h"
#include "bc_block_body/OmittedBlockBody.h"


namespace codablecash {

DownloadOmittedBlockBodyNodeCommand::DownloadOmittedBlockBodyNodeCommand(const DownloadOmittedBlockBodyNodeCommand &inst)
		: AbstractNodeCommand(inst) {
	this->zone = inst.zone;
	this->height = inst.height;
	this->headerId = inst.headerId != nullptr ? dynamic_cast<BlockHeaderId*>(inst.headerId->copyData()) : nullptr;
}

DownloadOmittedBlockBodyNodeCommand::DownloadOmittedBlockBodyNodeCommand()
	: AbstractNodeCommand(TYPE_NODE_DOWNLOAD_OMITTED_BLOCK_BODY) {
	this->zone = 0;
	this->height = 0;
	this->headerId = nullptr;
}

DownloadOmittedBlockBodyNodeCommand::~DownloadOmittedBlockBodyNodeCommand() {
	delete this->headerId;
}

int DownloadOmittedBlockBodyNodeCommand::binarySize() const {
	BinaryUtils::checkNotNull(this->headerId);

	int total = AbstractNodeCommand::binarySize();

	total += sizeof(this->zone);
	total += sizeof(this->height);
	total += this->headerId->binarySize();

	return total;
}

void DownloadOmittedBlockBodyNodeCommand::toBinary(ByteBuffer *buff) const {
	BinaryUtils::checkNotNull(this->headerId);

	AbstractNodeCommand::toBinary(buff);

	buff->putShort(this->zone);
	buff->putLong(this->height);
	this->headerId->toBinary(buff);
}

ByteBuffer* DownloadOmittedBlockBodyNodeCommand::getSignBinary() const {
	BinaryUtils::checkNotNull(this->headerId);

	int total =  sizeof(this->zone);
	total += sizeof(this->height);
	total += this->headerId->binarySize();

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true);
	buff->putShort(this->zone);
	buff->putLong(this->height);
	this->headerId->toBinary(buff);

	return buff;
}

void DownloadOmittedBlockBodyNodeCommand::fromBinary(ByteBuffer *buff) {
	AbstractNodeCommand::fromBinary(buff);

	this->zone = buff->getShort();
	this->height = buff->getLong();
	this->headerId = BlockHeaderId::fromBinary(buff);

	BinaryUtils::checkNotNull(this->headerId);
}

IBlockObject* DownloadOmittedBlockBodyNodeCommand::copyData() const noexcept {
	return new DownloadOmittedBlockBodyNodeCommand(*this);
}

bool DownloadOmittedBlockBodyNodeCommand::usePendingQueue() const noexcept {
	return false;
}

AbstractCommandResponse* DownloadOmittedBlockBodyNodeCommand::executeAsNode(BlockchainNodeHandshake *nodeHandShake
		, CodablecashNodeInstance *inst, bool suspend) const {
	DownloadOmittedBlockBodyNodeCommandResponse* response = new DownloadOmittedBlockBodyNodeCommandResponse(); __STP(response);
	{
		BlockchainController* ctrl = inst->getController();
		Block* block = ctrl->getBlocksHeightAt(this->zone, this->height, this->headerId); __STP(block);

		if(block != nullptr){
			BlockBody* body = block->getBody();

			// to omitted block
			OmittedBlockBody* obody = body->toOmittedBlockBody(); __STP(obody);
			response->setOmittedBlockBody(obody);
		}
	}

	return __STP_MV(response);
}

void DownloadOmittedBlockBodyNodeCommand::setZone(uint16_t zone) noexcept {
	this->zone = zone;
}

void DownloadOmittedBlockBodyNodeCommand::setHeight(uint64_t height) noexcept {
	this->height = height;
}

void DownloadOmittedBlockBodyNodeCommand::setHeaderId(const BlockHeaderId *headerId) {
	delete this->headerId;
	this->headerId = dynamic_cast<BlockHeaderId*>(headerId->copyData());
}

} /* namespace codablecash */
