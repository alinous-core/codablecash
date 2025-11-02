/*
 * SyncHeaderNodeCommand.cpp
 *
 *  Created on: 2023/11/06
 *      Author: iizuka
 */

#include "bc_p2p_cmd_node/SyncHeaderNodeCommand.h"

#include "bc_p2p/BlockchainZoneException.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc/ExceptionThrower.h"
#include "bc/CodablecashNodeInstance.h"

#include "bc_p2p_cmd_node/SyncHeaderNodeCommandResponse.h"

#include "base/StackRelease.h"
namespace codablecash {

SyncHeaderNodeCommand::SyncHeaderNodeCommand(const SyncHeaderNodeCommand &inst) : AbstractNodeCommand(inst) {
	this->zone = inst.zone;
	this->numRequestHeight = inst.numRequestHeight;
	this->startHeight = inst.startHeight;
}

SyncHeaderNodeCommand::SyncHeaderNodeCommand() : AbstractNodeCommand(TYPE_NODE_SYNC_HEADER) {
	this->zone = 0;
	this->numRequestHeight = 100;
	this->startHeight = 0;
}

SyncHeaderNodeCommand::~SyncHeaderNodeCommand() {

}

int SyncHeaderNodeCommand::binarySize() const {
	int total = AbstractNodeCommand::binarySize();

	total += sizeof(this->zone);
	total += sizeof(this->numRequestHeight);
	total += sizeof(this->startHeight);

	return total;
}

void SyncHeaderNodeCommand::toBinary(ByteBuffer *buff) const {
	AbstractNodeCommand::toBinary(buff);

	buff->putShort(this->zone);
	buff->putInt(this->numRequestHeight);
	buff->putLong(this->startHeight);
}

ByteBuffer* SyncHeaderNodeCommand::getSignBinary() const {
	int total = sizeof(this->zone);
	total += sizeof(this->numRequestHeight);
	total += sizeof(this->startHeight);

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true);

	buff->putShort(this->zone);
	buff->putInt(this->numRequestHeight);
	buff->putLong(this->startHeight);

	return buff;
}

void SyncHeaderNodeCommand::fromBinary(ByteBuffer *buff) {
	AbstractNodeCommand::fromBinary(buff);

	this->zone = buff->getShort();
	this->numRequestHeight = buff->getInt();
	this->startHeight = buff->getLong();
}

IBlockObject* SyncHeaderNodeCommand::copyData() const noexcept {
	return new SyncHeaderNodeCommand(*this);
}

AbstractCommandResponse* SyncHeaderNodeCommand::executeAsNode(BlockchainNodeHandshake *nodeHandShake, CodablecashNodeInstance *inst, bool suspend) const {
	uint16_t zoneSelf = inst->getZoneSelf();
	ExceptionThrower<BlockchainZoneException>::throwExceptionIfCondition(zoneSelf != this->zone, L"Wrong zone.", __FILE__, __LINE__);

	SyncHeaderNodeCommandResponse* respose = new SyncHeaderNodeCommandResponse(); __STP(respose);

	{
		BlockchainController* ctrl = inst->getController();

		ctrl->getSyncHeaderData(zoneSelf, this->startHeight, this->numRequestHeight, respose);
	}

	return __STP_MV(respose);
}

void SyncHeaderNodeCommand::setZone(uint16_t zone) noexcept {
	this->zone = zone;
}

void SyncHeaderNodeCommand::setNumRequestHeight(uint32_t numRequestHeight) noexcept {
	this->numRequestHeight = numRequestHeight;
}

void SyncHeaderNodeCommand::setStartHeight(uint32_t offset) noexcept {
	this->startHeight = offset;
}

bool SyncHeaderNodeCommand::usePendingQueue() const noexcept {
	return false;
}

} /* namespace codablecash */
